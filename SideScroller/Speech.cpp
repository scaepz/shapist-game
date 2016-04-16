#include "Speech.h"


CSpeech::CSpeech(CAnimate * _speaker, SDL_Texture * _texture, int _timeToLive, SDL_Rect _dst)
{
	speaker = _speaker;
	texture = _texture;
	timeToLive = _timeToLive;
	dst = _dst;
}


CSpeech::~CSpeech()
{
	SDL_DestroyTexture(texture);
}
