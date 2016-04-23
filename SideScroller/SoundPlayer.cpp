#include "SoundPlayer.h"
#include <iostream>
#include "SoundEnum.h"

CSoundPlayer::CSoundPlayer()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) == -1)
	{
		std::cout << "SDL_mixer failed to init";
	}
	LoadCommonSounds();
	
}
void CSoundPlayer::PlaySoundAtVolume(int soundId, int loops, int volume)
{
	if (volume < 1) return;
	if (volume > 128) volume = 128;
	int channels = 8;
	//find free channel
	for (int i = 0; i < channels; i++)
	{
		if (Mix_Playing(i) == false)
		{
			Mix_Volume(i, volume);
			Mix_PlayChannel(i, soundMap[soundId], loops);			
			return;
		}
	}
	//if none free, steal a random one
	int i = rand() % 8;
	Mix_Volume(i, volume);
	Mix_PlayChannel(i, soundMap[soundId], loops);
}
int CSoundPlayer::GetVolumeByDistance(int distance)
{
	if (distance == 0) return maxVolume;
	return  -1* (maxVolume*(cbrt((float)distance/(float)soundMaxDistance)-1));
}
//returns the channel
int CSoundPlayer::PlaySound(int soundId, int loops)
{
	return Mix_PlayChannel(-1, soundMap[soundId], loops);
}

//stops a channel
void CSoundPlayer::StopSound(int channel)
{
	Mix_HaltChannel(channel);
}

int CSoundPlayer::PlayMs(int soundId, int loops ,int ms)
{
	return Mix_PlayChannelTimed(-1, soundMap[soundId], loops, ms);
}

void CSoundPlayer::LoadCommonSounds()
{
	soundMap[weapon_hit_high] = Mix_LoadWAV("res/sounds/weapon_hit_high.wav");
	soundMap[weapon_hit_low] = Mix_LoadWAV("res/sounds/weapon_hit_low.wav");
	soundMap[weapon_hit_low_quick] = Mix_LoadWAV("res/sounds/weapon_hit_low_quick.wav");
	soundMap[weapon_hit_medium] = Mix_LoadWAV("res/sounds/weapon_hit_medium.wav");
	soundMap[shotgun_blast] = Mix_LoadWAV("res/sounds/shotgun.wav");
	soundMap[heavyShotgun_blast] = Mix_LoadWAV("res/sounds/heavyShotgun.wav");
	soundMap[ak47_shot] = Mix_LoadWAV("res/sounds/ak_shot.wav");
	soundMap[grenadeLauncher_fire] = Mix_LoadWAV("res/sounds/grenadeLauncher_fire.wav");
	soundMap[m4_shot] = Mix_LoadWAV("res/sounds/m4_shot.wav");
	soundMap[explosion_small] = Mix_LoadWAV("res/sounds/explosion_small.wav");
	soundMap[explosion_medium] = Mix_LoadWAV("res/sounds/explosion_medium.wav");
	soundMap[silenced_shot] = Mix_LoadWAV("res/sounds/silenced_shot.wav");
	soundMap[glock_shot] = Mix_LoadWAV("res/sounds/glock_shot.wav");
	soundMap[mp5_shot] = Mix_LoadWAV("res/sounds/mp5_shot.wav");
	soundMap[revolver_shot] = Mix_LoadWAV("res/sounds/revolver_shot.wav");
	soundMap[mg_shot] = Mix_LoadWAV("res/sounds/mg_shot.wav");
	soundMap[heavyMg_shot] = Mix_LoadWAV("res/sounds/heavymg_shot.wav");
	soundMap[lightSniper_shot] = Mix_LoadWAV("res/sounds/lightsniper_shot.wav");

}

CSoundPlayer::~CSoundPlayer()
{
	for (std::map<int, Mix_Chunk*>::iterator i = soundMap.begin(); i != soundMap.end(); ++i)
	{
		Mix_FreeChunk(i->second);
		i->second = nullptr;
		
	}
}


void CSoundPlayer::PlayDamageSound(int damage)
{
	
	if (damage >= 0 && damage < 10) //white
	{
		Mix_PlayChannel(-1, soundMap[weapon_hit_low_quick], 0);
	}

	else if (damage >= 10 && damage < 30) // yellow
	{
		Mix_PlayChannel(-1, soundMap[weapon_hit_low], 0);
	}
	else if (damage >= 30 && damage < 70) //orange
	{
		Mix_PlayChannel(-1, soundMap[weapon_hit_medium], 0);
	}
	else if (damage >= 70) //red
	{
		Mix_PlayChannel(-1, soundMap[weapon_hit_high], 0);
	}
}