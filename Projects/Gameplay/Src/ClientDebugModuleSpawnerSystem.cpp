#include "ClientDebugModuleSpawnerSystem.h"
#include "InputBackendSystem.h"
#include <TcpClient.h>
#include "SpawnDebugModulePacket.h"
#include "EntityType.h"
#include "Transform.h"

ClientDebugModuleSpawnerSystem::ClientDebugModuleSpawnerSystem(TcpClient* p_client)
	: EntitySystem(SystemType::ClientDebugModuleSpawnerSystem, 3,
	ComponentType::Transform, ComponentType::TAG_MyShip,
	ComponentType::MeshOffsetTransform)
{
	m_client = p_client;
}

void ClientDebugModuleSpawnerSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		if(m_input->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0.0)
		{
			Transform* transform = static_cast<Transform*>(p_entities[i]->getComponent(
				ComponentType::Transform));
			SpawnDebugModulePacket data;
			data.shipPosition = transform->getTranslation();
			data.moduleTypes[0] = EntityType::ShieldModule;
			data.moduleTypes[1] = EntityType::AnomalyModule;
			data.moduleTypes[2] = EntityType::BoosterModule;
			data.moduleTypes[3] = EntityType::MineLayerModule;
			data.moduleTypes[4] = EntityType::MinigunModule;
			data.moduleTypes[5] = EntityType::TeslaCoilModule;
			data.numberOfModules = 6;
			m_client->sendPacket(data.pack());
		}
	}
}

void ClientDebugModuleSpawnerSystem::initialize()
{
	m_input = static_cast<InputBackendSystem*>(m_world->getSystem(
		SystemType::InputBackendSystem));
}