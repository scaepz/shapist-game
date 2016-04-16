#ifndef DAMAGENUMBER_H
#define DAMAGENUMBER_H
#include <SDL.h>
#include <string>
#include "Global.h"
using std::string;
class CDamageNumber
{
public:
	CDamageNumber(int value, int startingPosX, int startingPosY);
	CDamageNumber(string value, int startingPosX, int startingPosY);
	virtual ~CDamageNumber();
	SDL_Color GetColor();
	bool Update();
	int GetFontIndex();
	string ToString();
	int GetX();
	int GetY();

private:

	int font;
	int timeAlive; // in ms
	SDL_Color color;
	string value;
	float position[2];
	float velocity[2];
	
};

#endif