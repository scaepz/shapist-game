#ifndef SPEECH_H
#define SPEECH_H
#include "Animate.h"
#include "SDL.h"
class CSpeech
{
public:
	CAnimate * speaker;
	SDL_Rect dst;
	SDL_Texture * texture;
	int timeToLive;
	CSpeech(CAnimate * _speaker, SDL_Texture * _texture, int _timeToLive, SDL_Rect dst);
	virtual ~CSpeech();
};

#endif