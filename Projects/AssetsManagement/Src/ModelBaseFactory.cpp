#include "ModelBaseFactory.h"
#include <AglReader.h>
#include <AglScene.h>
#include "MeshNameScriptParser.h"


ModelBaseFactory::ModelBaseFactory()
{
	m_modelResourceCache = new ResourceManager<ModelResourceCollection>;
}

ModelBaseFactory::~ModelBaseFactory()
{
	delete m_modelResourceCache;
}


ModelResource* ModelBaseFactory::createModelResource_DEPRECATED( const string& p_name, 
												 const string* p_path/*=NULL*/)
{
	ModelResource* model = NULL;
	// Check and read the file
	vector<ModelResource*>* models = createModelResources( p_name, p_path);
	model = (*models)[0];
	return model;
}

vector<ModelResource*>* ModelBaseFactory::createModelResources( const string& p_name, 
														const string* p_path/*=NULL*/)
{
	InstanceInstruction currentInstance={p_name,AglMatrix::identityMatrix()};
	//
	vector<ModelResource*>* models = NULL;
	vector<InstanceInstruction>* instanceInstructions = new vector<InstanceInstruction>();
	int instanceCount = 0;
	// Check and read the file
	do 
	{	
		if (!instanceInstructions->empty()) instanceInstructions->pop_back();

		int modelFoundId = m_modelResourceCache->getResourceId(currentInstance.filename);
		// ---------------------
		// New mesh
		// ---------------------
		if (modelFoundId==-1)  // if it does not exist, create new
		{
			AglScene* scene = readScene(currentInstance.filename,p_path);
			//
			if (scene)
			{ 
				// DEBUGWARNING(( ("Loading meshes from "+currentInstance.filename+" instance="+toString(instanceCount)).c_str() ));
				if (instanceCount==0) 
				{
					// just a normal mesh, just copy resource instructions
					models = createAllModelData(&currentInstance,
						scene,
						instanceInstructions);		
					// read leftover empties
					if ((*models)[0]!=NULL)
					{
						ModelResource* model = (*models)[0];
						SourceData source={scene,NULL,NULL,-1,string("")};
						readAndStoreEmpties(source,model,currentInstance.transform,
							&currentInstance,instanceInstructions); 
					}
				}
				else
				{
					// an instance needs to add(instead of copy) to original collection from 
					// the instance's read collection.
					// it also needs to copy the model resource data
					vector<ModelResource*>* prefetched = createAllModelData(&currentInstance,
						scene,
						instanceInstructions);
					// read leftover empties
					if ((*prefetched)[0]!=NULL)
					{
						ModelResource* model = (*prefetched)[0];
						SourceData source={scene,NULL,NULL,-1,string("")};
						readAndStoreEmpties(source,model,currentInstance.transform,
											&currentInstance,instanceInstructions); 
					}
					//
					ModelResourceCollection* modelresourceCollection = m_modelResourceCache->getResource(currentInstance.filename);
					unsigned int start = modelresourceCollection->rootIndex+1;
					unsigned int size = prefetched->size();
					for (unsigned int n=start;n<size;n++)
					{
						ModelResource* model = new ModelResource( *(*prefetched)[n] );
						// mesh transform
						model->transform = model->transform*currentInstance.transform;
						// 
						models->push_back(model);
					}
				}

			}
			else
			{
				models->push_back(getFallback());
			}
			// cleanup
			delete scene;
		}
		else // the mesh already exists
		{
		// ---------------------
		// Existing mesh
		// ---------------------
			if (instanceCount==0) 
			{
				// just a normal mesh, just copy resource instructions
				models = &m_modelResourceCache->getResource(currentInstance.filename)->collection;
			}
			else					
			{
				// an instance needs to add to original collection from  the instance's collection
				// it also needs to copy the model resource data
				ModelResourceCollection* modelresourceCollection = m_modelResourceCache->getResource(currentInstance.filename);
				vector<ModelResource*>* prefetched = &modelresourceCollection->collection;

				unsigned int start = modelresourceCollection->rootIndex+1;
				unsigned int size = prefetched->size();
				for (unsigned int n=start;n<size;n++)
				{
					ModelResource* model = new ModelResource( *(*prefetched)[n] );
					// mesh transform
					model->transform *= currentInstance.transform;
					// instances
					for (unsigned int n=0;n<model->instances.size();n++)
					{
						InstanceInstruction instruction = model->instances[n];
						instruction.transform *= currentInstance.transform;
						instanceInstructions->push_back(instruction);
					}
					// 
					models->push_back(model);
				}
			}
		}

		// read and prepare next instance if file had any instances specified
		if (!instanceInstructions->empty())
		{
			currentInstance = instanceInstructions->back();
		}
		instanceCount++;

	} while (!instanceInstructions->empty());


	delete instanceInstructions;

	return models;
}

AglScene* ModelBaseFactory::readScene(const string& p_name, const string* p_path)
{
	string fullPath;
	if (p_path!=NULL) fullPath = *p_path;
	fullPath += p_name;
	// test file
	string fileChkMsg;
	if (!isFileOk(fullPath,fileChkMsg,__FILE__,__FUNCTION__,__LINE__))
		throw MeshLoadException(fileChkMsg);
	// read file and extract scene
	AglReader meshReader(fullPath.c_str());
	AglScene* aglScene = meshReader.getScene();
	return aglScene;
}

vector<ModelResource*>* ModelBaseFactory::createAllModelData( 
	const InstanceInstruction* p_instanceData, 
	AglScene* p_scene, 
	vector<InstanceInstruction>* p_outInstanceInstructions)
{
	ModelResourceCollection* models = new ModelResourceCollection();
	// if several models were found, or none, add a root entity
	unsigned int numberOfModels = p_scene->getMeshes().size();
	if (numberOfModels>1 || numberOfModels==0)
	{
		models->collection.push_back(new ModelResource(p_instanceData->filename+"-ROOT"));
		models->rootIndex=models->collection.size()-1;
	}
	// check all models
	for (unsigned int i=0; i<numberOfModels; i++)
	{	
		AglMesh* aglMesh = p_scene->getMeshes()[i];
		AglMeshHeader aglMeshHeader = aglMesh->getHeader();	
		// parse mesh name
		string meshName = p_scene->getName(aglMeshHeader.nameID);
		pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
		parsedAction = MeshNameScriptParser::parse(meshName);
		// DEBUGWARNING(( ("Creating mesh "+meshName).c_str() ));
		// Actions based on parsed name
		switch (parsedAction.second) 
		{
		case MeshNameScriptParser::INSTANTIATE: // instantiate
			{
				if (p_outInstanceInstructions!=NULL)
				{
					InstanceInstruction inst = {parsedAction.first.filename,
						aglMeshHeader.transform};

					DEBUGWARNING(((p_instanceData->filename+": Found mesh with instancing instruction. Conversion error?").c_str()));
					// Not possible yet-> p_model->instances.push_back(inst);
					p_outInstanceInstructions->push_back(inst);
				}

				break;
			}

		case MeshNameScriptParser::MESH: // normal mesh
		default:				
			{
				// DEBUGWARNING(( string("Normal mesh").c_str() ));
				SourceData source={p_scene,aglMesh,&aglMeshHeader,
					i,
					parsedAction.first.name};
				//
				createAndAddModel(models, 
					p_instanceData, 
					source, 
					p_outInstanceInstructions);
				break;
			}
		}

	}
	m_modelResourceCache->addResource(p_instanceData->filename,models); // register collection in cache

	return &models->collection;
}


void ModelBaseFactory::createAndAddModel( ModelResourceCollection* p_modelCollection, 
										 const InstanceInstruction* p_instanceData, 
										 ModelBaseFactory::SourceData& p_source,
										 vector<InstanceInstruction>* p_outInstanceInstructions)
{
	// set
	ModelResource* model = new ModelResource();
	model->meshHeader = *(p_source.meshHeader);
	p_source.nameSuffix = "_"+p_source.nameSuffix;
	if (p_source.modelNumber==0)
	{
		p_source.nameSuffix="";
	}
	// Mesh data
	unsigned int meshResultId = createMeshData( p_source,p_instanceData );
	// store in model
	model->name = p_instanceData->filename+p_source.nameSuffix;
	model->meshId = static_cast<int>(meshResultId);
	model->transform = p_source.meshHeader->transform;

	// other model creation data
	readAndStoreEmpties(p_source,
		model,
		model->transform,
		p_instanceData,
		p_outInstanceInstructions);
	readAndStoreParticleSystems(p_source,model);

	// Done
	p_modelCollection->collection.push_back(model); // register in collection
}


unsigned int ModelBaseFactory::createMeshData( ModelBaseFactory::SourceData& p_source,
											  const InstanceInstruction* p_instanceData)
{
	unsigned int meshResultId=-1;
	return meshResultId;
}



void ModelBaseFactory::readAndStoreEmpties( SourceData& p_source, 
										   ModelResource* p_model, 
										   AglMatrix& p_offset,
										   const InstanceInstruction* p_instanceData, 
										   vector<InstanceInstruction>* p_outInstanceInstructions)
{
	unsigned int connectionPoints = p_source.scene->getConnectionPointCount();
	for (unsigned int n=0;n<connectionPoints;n++)
	{
		AglConnectionPoint* cp = &p_source.scene->getConnectionPoint(n);
		// parse name
		string name = p_source.scene->getName(cp->nameID);
		pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
		parsedAction = MeshNameScriptParser::parse(name);
		// Actions based on parsed name
		switch (parsedAction.second) 
		{
		case MeshNameScriptParser::INSTANTIATE: // instantiate
			{
				if (cp->parentMesh == p_source.modelNumber) // handle global and local call the same
				{
					InstanceInstruction inst = {parsedAction.first.filename,
						cp->transform*p_offset};
					// DEBUGWARNING(( ("Found instance "+parsedAction.first.filename).c_str() ));
					p_model->instances.push_back(inst);
					p_outInstanceInstructions->push_back(inst);
				}
				break;
			}

		case MeshNameScriptParser::CONNECTIONPOINT: // normal cp
		default:				
			{
				if (p_source.modelNumber!=-1) // call from parent
				{
					if (cp->parentMesh == p_source.modelNumber)
					{
						// DEBUGWARNING(( string("Found connection point for mesh!").c_str() ));
						p_model->connectionPoints.m_collection.push_back(cp->transform*p_offset);
					}
				}
				else // call from global
				{
					// make pointed model to parent
					if (cp->parentMesh == -1 && p_model!=NULL)
					{
						// DEBUGWARNING(( string("Found global connection point!").c_str() ));
						p_model->connectionPoints.m_collection.push_back(cp->transform*p_offset);
					}
				}

				break;
			}

		}

	}
}

void ModelBaseFactory::readAndStoreParticleSystems( SourceData& p_source, 
													ModelResource* p_model)
{
	// ------------------
	// Particles
	// ------------------
	unsigned int particleSystems = p_source.scene->getParticleSystems().size();
	for (unsigned int n=0;n<particleSystems;n++)
	{
		AglParticleSystem* ps = p_source.scene->getParticleSystem(n);
		if (p_source.modelNumber==0) // add support for particle parent?
		{
			p_model->particleSystems.m_collection.push_back(*ps);
		}
	}
}

ModelResource* ModelBaseFactory::getFallback()
{
	// fallback mesh and texture
	ModelResource* model = new ModelResource();

	// Done
	return model;
}

AglLooseBspTree* ModelBaseFactory::createBspTree(AglMesh* p_mesh)
{
	//ANTON
	AglMeshHeader h = p_mesh->getHeader();
	AglVertexSTBN* v = (AglVertexSTBN*)p_mesh->getVertices();
	unsigned int* ind = p_mesh->getIndices();

	vector<AglVector3> vertices;
	vector<unsigned int> indices;
	for (int i = 0; i < h.vertexCount; i++)
	{
		vertices.push_back(v[i].position);
	}
	for (int i = 0; i < h.indexCount; i++)
	{
		indices.push_back(ind[i]);
	}
	AglLooseBspTreeConstructor constructor(h.id, vertices, indices);
	return constructor.createTree();
	//END ANTON
}

