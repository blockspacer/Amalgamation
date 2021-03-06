#include "SoundSystem.h"
#include "SoundComponent.h"
#include <SoundWrapper.h>
#include "TimerSystem.h"
#include "PositionalSoundSystem.h"

SoundSystem::SoundSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::SoundSystem, 1, ComponentType::SoundComponent)
{
	m_audioBackend = p_audioBackend;
}

SoundSystem::~SoundSystem()
{

}

void SoundSystem::initialize()
{
	m_timerSystem = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));
}

void SoundSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++){

		SoundComponent* soundComp = static_cast<SoundComponent*>
			(p_entities[i]->getComponent(ComponentType::SoundComponent));
		updateSoundStatus(soundComp);
	}
}

void SoundSystem::inserted( Entity* p_entity )
{
	SoundComponent* soundComp = static_cast<SoundComponent*>
		(p_entity->getComponent(ComponentType::SoundComponent));

	//Create all the new sounds using the soundwrapper
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < soundComp->m_sounds[i].size(); j++){
			unsigned soundIndex = m_audioBackend->getSoundWrapper()->createSoundFromHeader
				(soundComp->m_sounds[i][j]);

			soundComp->m_sounds[i][j]->soundIndex = soundIndex;
		}
	}

	updateSoundStatus(soundComp, true);

	PositionalSoundSystem* posSystem = static_cast<PositionalSoundSystem*>
		(m_world->getSystem(SystemType::PositionalSoundSystem));

	if(posSystem) posSystem->processSoundComponent(p_entity, soundComp);
}

void SoundSystem::removed( Entity* p_entity )
{
	SoundComponent* soundComp = static_cast<SoundComponent*>
		(p_entity->getComponent(ComponentType::SoundComponent));

	//Create all the new sounds using the soundwrapper
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < soundComp->m_sounds[i].size(); j++){
			AudioHeader* header = soundComp->m_sounds[i][j];
			m_audioBackend->getSoundWrapper()->destroySound(header->soundIndex);
		}
	}
}

void SoundSystem::updateSoundStatus(const SoundComponent* p_soundComponent, 
									bool p_forceUpdate/* =false */)
{
	for(unsigned int i = 0; i < AudioHeader::SoundType::NUMSOUNDTYPES; i++){
		for(unsigned int j = 0; j < p_soundComponent->m_sounds[i].size(); j++){
			AudioHeader* header = p_soundComponent->m_sounds[i][j];

			if(p_forceUpdate || m_timerSystem->checkTimeInterval(TimerIntervals::Every32Millisecond)){
				if(header->maxFrequencyOffeset!=1.0f){
					//Update the frequency 
					m_audioBackend->getSoundWrapper()->
						getSound(header->soundIndex)->setFrequency(header->frequency);
				}

				//Update the volume
				if(header->soundType != AudioHeader::AMBIENTRANGE){
					m_audioBackend->getSoundWrapper()->getSound(header->soundIndex)->
						getSourceVoice()->SetVolume(header->volume);
				}
			}
			//Update the sounds' playing status 
			if( header->getPlayingState() != header->queuedPlayingState ){
				header->setPlayingState(header->queuedPlayingState);
				m_audioBackend->changeAudioInstruction(header->soundIndex, 
					header->getPlayingState());

				if (header->playInterval == AudioHeader::ONCE){
					header->setPlayingState( AudioHeader::STOP);
					header->queuedPlayingState = AudioHeader::STOP;
				}
			}

			//Update timer based sounds
			if(header->playInterval == AudioHeader::TIMERBASED){

				if (header->getPlayingState() == AudioHeader::PLAY){
					header->timeSinceLastPlayed += m_world->getDelta();

					if(header->timeSinceLastPlayed >= header->timerInterval){
						m_audioBackend->changeAudioInstruction(header->soundIndex, 
							AudioHeader::PlayState::RESTART);
						header->timeSinceLastPlayed = 0;
					}
				}
			}
		}
	}
}
