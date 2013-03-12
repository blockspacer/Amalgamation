#include "ServerUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>
#include "TimerSystem.h"

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"
#include "EntityUpdatePacket.h"
#include "EntityDeletionPacket.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "PhysicsController.h"
#include "ParticleSystemServerComponent.h"
#include "ParticleUpdatePacket.h"
#include "UpdateClientStatsPacket.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
#include "PlayerComponent.h"
#include "ServerClientInfoSystem.h"
#include "ClientInfo.h"
#include "ServerStateSystem.h"
#include "CameraInfo.h"
#include "LookAtEntity.h"
#include "PlayerSystem.h"
#include "WinningConditionSystem.h"

ServerUpdateSystem::ServerUpdateSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkUpdateSystem, 2, ComponentType::NetworkSynced,
	ComponentType::Transform )
{
	m_server = p_server;
}

ServerUpdateSystem::~ServerUpdateSystem()
{
}

void ServerUpdateSystem::processEntities( const vector<Entity*>& p_entities )
{
	NetworkSynced* netSync = NULL;
	Transform* transform = NULL;
	PhysicsBody* physicsBody = NULL;

	auto timerSys = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));

	ServerStateSystem* stateSystem  = static_cast<ServerStateSystem*>(
		m_world->getSystem(SystemType::ServerStateSystem));

	switch (stateSystem->getCurrentState())
	{
	case ServerStates::INGAME:
		{
			if( timerSys->checkTimeInterval(TimerIntervals::EverySecond) )
			{
				UpdateClientStatsPacket updatedClientInfo;

				PlayerSystem* playerSys  = static_cast<PlayerSystem*>(
					m_world->getSystem(SystemType::PlayerSystem));

				WinningConditionSystem* winningSys = static_cast<WinningConditionSystem*>
					(m_world->getSystem(SystemType::WinningConditionSystem));

				vector<PlayerComponent*> players = playerSys->getPlayerComponents();
				updatedClientInfo.activePlayers = players.size();
				updatedClientInfo.minutesUntilEndOfRound = winningSys->getRemaningMinutes();
				updatedClientInfo.secondsUntilEndOfRound = winningSys->getRemaningSeconds();

				for(unsigned int i = 0; i < updatedClientInfo.activePlayers; i++){
					updatedClientInfo.scores[i] = players.at(i)->getScore();
					updatedClientInfo.ping[i] = players.at(i)->m_ping;
					updatedClientInfo.playerIdentities[i] = i;
				}

				updatedClientInfo.currentServerTimestamp = m_world->getElapsedTime();

				m_server->broadcastPacket(updatedClientInfo.pack());
			}
			// NOTE: (Johan) This interval check is currently set to be very high delay because
			// packet handling is too slow when running Debug build otherwise.
			TimerIntervals::Enum entityupdateInterval = TimerIntervals::Every16Millisecond;
#ifdef _DEBUG
			entityupdateInterval = TimerIntervals::Every64Millisecond;
#endif
			if( timerSys->checkTimeInterval(entityupdateInterval) )
			{
				for( unsigned int entityIdx=0; entityIdx<p_entities.size(); entityIdx++ )
				{
					int currentEntity = p_entities[entityIdx]->getIndex();
					netSync = static_cast<NetworkSynced*>(
						m_world->getComponentManager()->getComponent(
						p_entities[entityIdx]->getIndex(), ComponentType::NetworkSynced ) );

					if( true)//netSync->getNetworkType() == EntityType::MinigunModule )
					{
						ParticleSystemServerComponent* psServerComp = 
							static_cast<ParticleSystemServerComponent*>
							(p_entities[entityIdx]->getComponent( 
							ComponentType::ParticleSystemServerComponent ) );

						if( psServerComp != NULL )
						{
							for( unsigned int psIdx=0; psIdx<psServerComp->particleSystems.size(); psIdx++ )
							{
								AglParticleSystemHeader* updateData =
									&psServerComp->particleSystems[psIdx].updateData;
								bool hasChanged = true;
								if(m_previousParticles.count(updateData) > 0)
								{
									AglParticleSystemHeader& previousHeader =
										m_previousParticles[updateData].particleHeader;
									if(previousHeader.spawnPoint == updateData->spawnPoint &&
										previousHeader.spawnDirection == updateData->spawnDirection &&
										fabs(previousHeader.spawnSpeed - updateData->spawnSpeed) < 0.0001f &&
										fabs(previousHeader.spawnFrequency - updateData->spawnFrequency) < 0.0001f &&
										previousHeader.color == updateData->color)
									{
										hasChanged = false;
									}
								}

								if(hasChanged || m_world->getElapsedTime() >
									m_previousParticles[updateData].timestamp + 1.0f)
								{
									m_previousParticles[updateData].particleHeader =
										*updateData;
									m_previousParticles[updateData].timestamp =
										m_world->getElapsedTime();
									ParticleUpdatePacket updatePacket;
									updatePacket.networkIdentity	= netSync->getNetworkIdentity();
									updatePacket.particleSystemIdx	= psIdx;
									updatePacket.position			= updateData->spawnPoint;
									updatePacket.direction			= updateData->spawnDirection;
									updatePacket.speed				= updateData->spawnSpeed;
									updatePacket.spawnFrequency		= updateData->spawnFrequency;
									updatePacket.color				= updateData->color;

									if (updatePacket.spawnFrequency == -1)
										updateData->spawnFrequency = psServerComp->particleSystems[psIdx].originalSettings.spawnFrequency;

									updatePacket.forceParticleMove = false;
									if (updateData->particleSpace == (char)AglParticleSystemHeader::AglSpace_SCREEN)
										updatePacket.forceParticleMove = true;
									m_server->broadcastPacket( updatePacket.pack() );
								}
							}
						}
					}
					//else
					{
						transform = static_cast<Transform*>(
							m_world->getComponentManager()->getComponent(
							p_entities[entityIdx]->getIndex(), ComponentType::Transform ) );

						bool hasChanged = true;
						if(m_previousTransforms.count(currentEntity) > 0)
						{
							Transform& prevTransform = m_previousTransforms[currentEntity].transform;
							if(prevTransform.getTranslation() == transform->getTranslation() &&
								prevTransform.getRotation() == transform->getRotation() &&
								prevTransform.getScale() == transform->getScale())
							{
								hasChanged = false;
							}
						}

						if(hasChanged || m_world->getElapsedTime() > 
							m_previousTransforms[currentEntity].timestamp + 1.0f)
						{
							m_previousTransforms[currentEntity].transform = *transform;
							m_previousTransforms[currentEntity].timestamp = 
								m_world->getElapsedTime();
							EntityUpdatePacket updatePacket;
							updatePacket.networkIdentity = netSync->getNetworkIdentity();
							updatePacket.entityType		= static_cast<char>(netSync->getNetworkType());
							updatePacket.translation	= transform->getTranslation();
							updatePacket.rotation		= transform->getRotation();
							updatePacket.scale			= transform->getScale();
							updatePacket.timestamp		= m_world->getElapsedTime();
							Packet packet((char)PacketType::EntityUpdate);
							packet.WriteData(&updatePacket, sizeof(EntityUpdatePacket));

							m_server->broadcastPacket( packet );
						}
					}
				}
				//Broadcast an end of the batch
				EntityUpdatePacket updatePacket;
				updatePacket.entityType		= static_cast<char>(EntityType::EndBatch);
				Packet packet((char)PacketType::EntityUpdate);
				packet.WriteData(&updatePacket, sizeof(EntityUpdatePacket));

				m_server->broadcastPacket( packet );
			}
			break;
		}
	default:
		break;
	}

}

void ServerUpdateSystem::initialize()
{
}

void ServerUpdateSystem::removed( Entity* p_entity )
{
	EntityDeletionPacket packet;
	auto netSync = static_cast<NetworkSynced*>(
		p_entity->getComponent(ComponentType::NetworkSynced));

	packet.networkIdentity = netSync->getNetworkIdentity();
	m_server->broadcastPacket(packet.pack());
}
