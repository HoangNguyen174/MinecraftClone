#include "AudioSystem.hpp"

AudioSystem::AudioSystem()
{

}

AudioSystem::~AudioSystem()
{

}

void AudioSystem::Initialize()
{
	FMOD::System_Create(&m_system);
	m_system->init(32,FMOD_INIT_NORMAL,0);
	m_secondRemainingTillNextSound = 0.f;
}

FMOD::Sound* AudioSystem::CreateOrLoadSound(char* fileName)
{
	map< const char*, FMOD::Sound* >::iterator it ;
	for( it = m_soundList.begin(); it != m_soundList.end(); ++it )
	{
		if(it->first == fileName)
		{
			return it->second;
		}
	}
	
	FMOD::Sound* newSound = nullptr;
	m_system->createSound(fileName, FMOD_DEFAULT, 0, &newSound);
	m_soundList[ fileName ] = newSound;

	return newSound;
}

void AudioSystem::PlaySoundByName(char* soundName, int volume, bool loop)
{
	FMOD::Sound* audioStream = CreateOrLoadSound(soundName);
	FMOD::Channel* audioChannel;
	
	m_system->playSound(FMOD_CHANNEL_FREE, audioStream, false, &audioChannel);

	audioChannel->setVolume(volume);
	if(loop)
		audioChannel->setMode(FMOD_LOOP_NORMAL);
}


void AudioSystem::UpdateTimer(float elapsedTime)
{
	if(m_secondRemainingTillNextSound > 0.f)
		m_secondRemainingTillNextSound -= elapsedTime;
}

void AudioSystem::SetTimer(float playerSpeed)
{
	m_secondRemainingTillNextSound = ( static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.25f)) ) + 0.25f;

	m_secondRemainingTillNextSound = m_secondRemainingTillNextSound ;
}

void AudioSystem::Update()
{
	m_system->update();
}




