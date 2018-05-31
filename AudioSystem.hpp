#ifndef AUDIO_SYSTEMH
#define AUDIO_SYSTEMH

#include "fmod.hpp"
#include "MathUtilities.hpp"
#include <vector>
#include "Timer.h"
#include <time.h>
#include <map>
#include <string.h>

#pragma comment( lib, "fmodex_vc.lib" )

class AudioSystem
{
	public:
		FMOD::System* m_system;
		FMOD::Sound* m_walkingOnGrass;
		float m_secondRemainingTillNextSound;
		Timer m_timer;
		map<const char*,FMOD::Sound*> m_soundList;
		
	public:
		AudioSystem();
		~AudioSystem();
		void Initialize();
		FMOD::Sound* CreateOrLoadSound(char* soundName);
		void PlaySoundByName(char* soundName, int volume, bool loop);
		void UpdateTimer(float elapsedTime);
		void SetTimer(float playerSpeed);
		void Update();
};
#endif