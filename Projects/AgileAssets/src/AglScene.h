// =================================================================================================
//                                         AglScene
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Scene containing all data supported by the format.
///
/// 
/// # AglScene
/// Edited By: Anton Andersson, 2012-11-27
///-------------------------------------------------------------------------------------------------

#ifndef AGLSCENE_H
#define AGLSCENE_H

#include "Agl.h"
#include "AglAnimationLayer.h"
#include "AglNodeAnimation.h"
#include "AglAnimation.h"
#include "AglSkeleton.h"
#include "AglGradient.h"
#include "AglMesh.h"
#include "AglSkeletonMapping.h"
#include "AglLooseBspTree.h"
#include "AglInteriorSphereGrid.h"
#include <vector>

using namespace std;

struct AglSceneDesc
{
	vector<AglMesh*>				meshes;
	vector<AglNode>					nodes;
	vector<AglAnimationLayer*>		animationLayers;
	vector<AglNodeAnimation*>		nodeAnimations;
	vector<AglAnimation*>			animations;
	vector<AglSkeleton*>			skeletons;
	vector<AglMaterial*>			materials;
	vector<string>					names;
	vector<AglMaterialMapping>		materialMappings;
	vector<AglSkeletonMapping*>		skeletonMappings;
	vector<AglLooseBspTree*>		bspTrees;
	vector<AglInteriorSphereGrid*>	sphereGrids;
	string							textureDirectory;
};

class AglScene
{
private:
	vector<AglMesh*>				m_meshes; ///< List of meshes
	vector<AglNode>					m_nodes; ///< List of nodes
	vector<AglDynamicNode>			m_dynamicNodes; ///< List of animated nodes
	vector<AglAnimationLayer*>		m_animationLayers; ///< List of animation layers
	vector<AglNodeAnimation*>		m_nodeAnimations; ///< List of node animations
	vector<AglAnimation*>			m_animations; ///< List of animations
	vector<AglSkeleton*>			m_skeletons; ///< List of skeletons
	vector<AglMaterial*>			m_materials; ///< List of materials
	vector<AglGradient*>			m_gradients; ///< List of gradients
	vector<string>					m_names; ///< List of names
	vector<AglMaterialMapping>		m_materialMappings; ///< List of material mappings
	vector<AglSkeletonMapping*>		m_skeletonMappings; ///< List of skeleton mappings
	vector<AglLooseBspTree*>		m_bspTrees;			///< List of Bsp trees
	vector<AglInteriorSphereGrid*>	m_sphereGrids;		///< List of sphere grids

	unsigned int m_currentAnimation; ///< Currently played animation
public:
	///
	/// Default Constructor
	///
	AglScene();

	///
	/// Destructor
	///
	~AglScene();

	///
	/// Initializes the scene
	/// \param p_desc Contains the data needed for initialization.
	///
	void init(AglSceneDesc pDesc);
	
	///
	/// Gets a node at a certain index
	/// \param p_index The index
	/// \return The node at position p_index
	///
	AglNode					getNode(int p_index);

	///
	/// Gets an animated node at a certain index
	/// \param p_index The index
	/// \return The animated node at position p_index
	///
	AglDynamicNode			getDynamicNode(int p_index);

	///
	/// Gets an animation layer at a certain index
	/// \param p_index The index
	/// \return The animation layer at position p_index
	///
	AglAnimationLayer*		getAnimationLayer(int p_index);

	///
	/// Gets a node animation at a certain index
	/// \param p_index The index
	/// \return The node animation at position p_index
	///
	AglNodeAnimation*		getNodeAnimation(int p_index);

	///
	/// Gets an animation at a certain index
	/// \param p_index The index
	/// \return The animation at position p_index
	///
	AglAnimation*			getAnimation(int p_index);

	///
	/// Gets a skeleton at a certain index
	/// \param p_index The index
	/// \return The skeleton at position p_index
	///
	AglSkeleton*			getSkeleton(int p_index);

	///
	/// Gets a material at a certain index
	/// \param p_index The index
	/// \return The material at position p_index
	///
	AglMaterial*			getMaterial(int p_index);

	///
	/// Gets a name at a certain index
	/// \param p_index The index
	/// \return The name at position p_index
	///
	string					getName(int p_index);

	///
	/// Gets a gradient at a certain index
	/// \param p_index The index
	/// \return The gradient at position p_index
	///
	AglGradient*			getGradient(int p_index);

	///
	/// Gets all materials
	/// \return The materials
	///
	vector<AglMaterial*>	getMaterials();

	///
	/// Gets all gradients
	/// \return The gradients
	///
	vector<AglGradient*>	getGradients();

	///
	/// Gets all meshes
	/// \return The meshes
	///
	vector<AglMesh*>		getMeshes();

	///
	/// Gets all skeleton mappings
	/// \return The skeleton mappings
	///
	vector<AglSkeletonMapping*> getSkeletonMappings();

	///
	/// Gets all material mappings
	/// \return The material mappings
	///
	vector<AglMaterialMapping> getMaterialMappings();

	///-----------------------------------------------------------------------------------
	/// Get all the bsp trees
	/// \return The bsp trees in the scene
	///-----------------------------------------------------------------------------------
	vector<AglLooseBspTree*> getBspTrees();

	///-----------------------------------------------------------------------------------
	/// Get all the interior sphere grids
	/// \return The interior sphere grids in the scene
	///-----------------------------------------------------------------------------------
	vector<AglInteriorSphereGrid*> getSphereGrids();

	///
	/// Gets all scene data
	/// \return The scene data
	///
	AglSceneDesc getSceneData();

	/// 
	/// Get the number of skeletons in the editor
	/// \return The skeleton count
	/// 
	unsigned int getSkeletonCount();

	///
	/// Adds a gradient to the scene
	/// \param p_gradient The gradient
	///
	void addGradient(AglGradient* p_gradient);

	///
	/// Appends a transform to a dynamic node. SHOULD BE MOVED!
	/// \param p_index Index to the dynamic node
	/// \param p_transform The transform to append
	///
	void appendTransform(int p_index, AglMatrix p_transform);

	///
	/// Updates the scene
	/// \param p_dt Elapsed time.
	///
	void update(float p_dt);

	///
	/// Adds a name to the scene.
	/// \param p_name The name to add.
	/// \return The index to the name in the name array
	///
	int	 addName(string pName);

	///
	/// Adds a name to the scene.
	/// \param p_name The name to add.
	///
	void addMesh(AglMesh* pMesh);

	///
	/// Adds a material to the scene.
	/// \param p_material The material to add.
	///
	void addMaterial(AglMaterial* p_material);

	///
	/// Adds a material mapping to the scene.
	/// \param p_materialMapping The material mapping to add.
	///
	void addMaterialMapping(AglMaterialMapping p_materialMapping);

	///
	/// Adds a node to the scene.
	/// \param p_node The node to add.
	///
	void addNode(AglNode p_node);

	///
	/// Adds a skeleton to the scene.
	/// \param p_skeleton The skeleton to add.
	///
	void addSkeleton(AglSkeleton* p_skeleton);

	///
	/// Adds a skeleton mapping to the scene.
	/// \param p_skeletonMapping The skeleton mapping to add.
	///
	void addSkeletonMapping(AglSkeletonMapping* p_skeletonMapping);

	///
	/// Adds a node animation to the scene.
	/// \param p_nodeAnimation The node animation to add.
	///
	void addNodeAnimation(AglNodeAnimation* p_nodeAnimation);

	///
	/// Adds an animation layer to the scene.
	/// \param p_layer The layer to add.
	///
	void addAnimationLayer(AglAnimationLayer* p_layer);

	///
	/// Adds an animation to the scene.
	/// \param p_animation The animation to add.
	///
	void addAnimation(AglAnimation* p_animation);

	///-----------------------------------------------------------------------------------
	/// Adds a bsp tree to the scene
	/// \param p_bspTree The tree to add
	///-----------------------------------------------------------------------------------
	void addBspTree(AglLooseBspTree* p_bspTree);

	///-----------------------------------------------------------------------------------
	/// Adds a sphere grid to the scene
	/// \param The sphere grid to add
	///-----------------------------------------------------------------------------------
	void addSphereGrid(AglInteriorSphereGrid* p_sphereGrid);
};

#endif