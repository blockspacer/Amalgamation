#include "ClientDebugModuleSpawnerSystem.h"
#include "InputBackendSystem.h"
#include <TcpClient.h>
#include "SpawnDebugModulePacket.h"
#include "EntityType.h"

ClientDebugModuleSpawnerSystem::ClientDebugModuleSpawnerSystem(TcpClient* p_client)
	: EntitySystem(SystemType::ClientDebugModuleSpawnerSystem, 3,
	ComponentType::Transform, ComponentType::TAG_MyShip,
	ComponentType::MeshOffsetTransform)
{
	m_client = p_client;
}

void ClientDebugModuleSpawnerSystem::process()
{
	if(m_input->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0.0)
	{
		SpawnDebugModulePacket data;
		data.moduleType = EntityType::ShieldModule;
		m_client->sendPacket(data.pack());
	}
}

void ClientDebugModuleSpawnerSystem::initialize()
{
	m_input = static_cast<InputBackendSystem*>(m_world->getSystem(
		SystemType::InputBackendSystem));
}