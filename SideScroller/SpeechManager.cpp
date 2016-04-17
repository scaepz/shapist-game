#include "SpeechManager.h"


CSpeechManager::CSpeechManager(CVectorManager * _vm)
{
	font = TTF_OpenFont("res/fonts/AkizaSans-Regular.otf", 16);
	vm = _vm;
}


CSpeechManager::~CSpeechManager()
{
	TTF_CloseFont(font);
}

void CSpeechManager::AddSpeech(CAnimate* speaker, std::string text, SDL_Color color)
{
	SDL_Surface * surface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(vm->GetRenderer(), surface);
	SDL_FreeSurface(surface);
	SDL_Rect dst;

	TTF_SizeText(font, text.c_str(), &dst.w, &dst.h);

	CSpeech * speech = new CSpeech(speaker, texture, 2000, dst);
	vm->AddObject(speech);
}
void CSpeechManager::Update()
{
	vector<CSpeech*>* sv = vm->GetSpeechVector();
	for (int i = 0; i < sv->size(); i++)
	{
		sv->at(i)->timeToLive -= g_time;
		if (sv->at(i)->timeToLive <= 0)
		{
			delete sv->at(i);
			sv->erase(sv->begin() + i);
			i--;
		}
		else
		{
			sv->at(i)->dst.x = sv->at(i)->speaker->GetX() + sv->at(i)->speaker->GetWidth() / 2 - sv->at(i)->dst.w / 2;
			sv->at(i)->dst.y = sv->at(i)->speaker->GetY() - sv->at(i)->dst.h * 2;
		}
	}
}