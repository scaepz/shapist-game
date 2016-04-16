#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H
#include "Speech.h"
#include "VectorManager.h"
#include <string>
#include "SDL_ttf.h"

static class CSpeechManager
{
public:
	void AddSpeech(CAnimate* speaker, std::string text, SDL_Color color);
	void UpdateSpeech();
	CSpeechManager(CVectorManager * vm);
	virtual ~CSpeechManager();
private:	
	TTF_Font * font;
	CVectorManager * vm;
};

#endif