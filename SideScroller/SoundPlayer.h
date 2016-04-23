#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <map>
#include "SDL_mixer.h"
class CSoundPlayer
{
public:
	CSoundPlayer();
	virtual ~CSoundPlayer();

	void PlaySoundAtVolume(int soundId, int loops, int volume);

	//returns the channel
	int PlaySound(int soundId, int loops);

	//stops a channel
	void StopSound(int channel);

	//creates the correct damage sound
	void PlayDamageSound(int damage);

	//plays sound for ms milliseconds
	int PlayMs(int soundId, int loops, int ms);

	int maxVolume = 128;

	int GetVolumeByDistance(int distance);
	int soundMaxDistance = 600;

private:
	std::map<int, Mix_Chunk*> soundMap;
	// sounds that apply to every level
	void LoadCommonSounds();
	int currentVolume;
	
};


#endif // !SOUNDPLAYER_H
