#include "ModuleVisualEffectServerBufferSystem.h"
#include "TimerSystem.h"
#include <TcpServer.h>
#include "NetworkSynced.h"
#include <DebugUtil.h>

ModuleVisualEffectServerBufferSystem::ModuleVisualEffectServerBufferSystem(TcpServer* p_server, 
															   ServerStateSystem* p_states) :
	EntitySystem(SystemType::ModuleVisualEffectBufferSystem)
{
	m_server = p_server;
	m_serverStates = p_states;
}

ModuleVisualEffectServerBufferSystem::~ModuleVisualEffectServerBufferSystem()
{

}

void ModuleVisualEffectServerBufferSystem::initialize()
{

}

void ModuleVisualEffectServerBufferSystem::process()
{
	// Dequeue all packets and send to their clients
	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::Every8Millisecond))
	{
		while(!m_scoreFXqueue_entity.empty())
		{
			auto infoPair = m_scoreFXqueue_entity.front();

			auto netSync = static_cast<NetworkSynced*>(
				infoPair.first->getComponent(ComponentType::NetworkSynced));

			m_server->unicastPacket(infoPair.second.pack(), 
				netSync->getNetworkOwner());

			m_scoreFXqueue_entity.pop();
		}
		while(!m_scoreFXqueue_netowner.empty())
		{
			auto infoPair = m_scoreFXqueue_netowner.front();

			if (infoPair.first!=-1)
			{
				m_server->unicastPacket(infoPair.second.pack(), 
					infoPair.first);
			}


			m_scoreFXqueue_netowner.pop();
		}
		// status effects should be broadcasted to all however
		while(!m_statusFXqueue_netowner.empty())
		{
			auto info = m_statusFXqueue_netowner.front();
			// if (info.m_mode == ModuleStatusEffectPacket::SPAWNED)
				// DEBUGPRINT(("sent"));
			m_server->broadcastPacket(info.pack());
			m_statusFXqueue_netowner.pop();
		}
	}
}


void ModuleVisualEffectServerBufferSystem::enqueueEffect( Entity* p_entity, 
											OnHitScoreEffectPacket& p_packet )
{
	if (m_serverStates->getCurrentState()==ServerStates::INGAME)
		m_scoreFXqueue_entity.push(pair<Entity*,OnHitScoreEffectPacket>(p_entity,p_packet));
}

void ModuleVisualEffectServerBufferSystem::enqueueEffect(int p_networkOwnerId, 
											OnHitScoreEffectPacket& p_packet)
{
	if (m_serverStates->getCurrentState()==ServerStates::INGAME)
		m_scoreFXqueue_netowner.push(pair<int,OnHitScoreEffectPacket>(p_networkOwnerId,p_packet));
}

void ModuleVisualEffectServerBufferSystem::enqueueEffect( ModuleStatusEffectPacket& p_packet )
{
	if (m_serverStates->getCurrentState()==ServerStates::INGAME)
		m_statusFXqueue_netowner.push(p_packet);
}
