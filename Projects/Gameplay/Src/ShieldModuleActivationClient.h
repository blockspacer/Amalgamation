#pragma once
#include "ModuleEvent.h"
#include <Entity.h>
#include <vector>
#include "AudioBackendSystem.h"
#include "SoundComponent.h"
#include <Globals.h>
using namespace std;
// =======================================================================================
// ShieldModuleActivationClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShieldModuleActivationClient
/// Detailed description...
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------
class ShieldModuleActivationClient: public ModuleEvent
{
public:
	ShieldModuleActivationClient(vector<Entity*> p_plateEntities,
		Entity* p_shieldEntity, AudioBackendSystem* p_audioBackend)
	{
		m_plateEntities = p_plateEntities;
		m_shieldEntity = p_shieldEntity;
		m_audioBackend = p_audioBackend;
	}

	~ShieldModuleActivationClient()
	{
	}

	virtual void activate() final
	{
		for(unsigned int i=0; i<m_plateEntities.size(); i++)
		{
			m_plateEntities[i]->setEnabled(true);
		}
		if(m_shieldEntity->getComponent(ComponentType::SoundComponent) == NULL)
		{
			// RM-RT 2013-03-04
			/*
			m_shieldEntity->addComponent(new SoundComponent(
				TESTSOUNDEFFECTPATH, "Shield_Active.wav", true, 1.0f));
			m_shieldEntity->applyComponentChanges();
			*/
		}
	}

	virtual void deactivate() final
	{
		for(unsigned int i=0; i<m_plateEntities.size(); i++)
		{
			m_plateEntities[i]->setEnabled(false);
		}
		if(m_shieldEntity->getComponent(ComponentType::SoundComponent) != NULL)
		{
			m_shieldEntity->removeComponent(ComponentType::SoundComponent);
			m_shieldEntity->applyComponentChanges();
		}
	}

private:
	vector<Entity*> m_plateEntities;
	Entity* m_shieldEntity;
	AudioBackendSystem* m_audioBackend;

};