#ifndef ARTIST_H
#define ARTIST_H

#include <SDL_ttf.h>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include "PhysicsObject.h"
#include "Animate.h"
#include "BaseObject.h"
#include "Tile.h"
#include "Weapon.h"
#include "VectorManager.h"
#include "DialogHandler.h"
#include "Hud.h"
using std::string;
using std::vector;
class CArtist
{
public:
	int screenPos[2];
	int standardTileSize;
	float sizeMultiplier;
	CArtist(CVectorManager* vm, int _tileSet, int resX, int resY, string textureDataFilePath, CDialogHandler * dh, SDL_Renderer * renderer, SDL_Window * window);
	CVectorManager* vm;
	int tileSet;
	enum tileSets
	{
		test,
		cave
	};
	virtual ~CArtist();
	void DrawEverything();
	void FadeOut(int ms);
	void FadeIn(int ms);
private:
	struct background
	{
		int textureId;
		int height;
		int width;
		float distanceMultiplier;
	};
	void CalculateScreenPos();
	void DrawDialogs();
	void DrawParticles();
	void DrawTiles();
	void DrawObjects();
	void DrawBackground(background bg);
	void DrawHud();
	void DrawSpeech();
	int resolution[2];
	int input;
	std::vector<std::vector<CTile*>>* tileVector;
	std::vector<CBaseObject*>* baseVector;
	std::map<int, SDL_Texture*> textureMap;
	
	//for drawing weapons
	//
	vector<CWeapon*>* weaponVector;
	vector<CAnimate*>* animateVector;
	//

	CPlayer * player;
	SDL_Renderer * renderer;
	SDL_Window * window;
	SDL_Texture * levelTexture;
	string GetRow(const string & file, int row);


	//hud and speech and dialogs
	CHud * hud;

	CDialogHandler * dialogHandler;
	TTF_Font * speechFont;
	SDL_Surface* message;
	SDL_Texture * messageT;
	SDL_Texture * inventory;
	SDL_Texture * grenadeIcon;
	SDL_Texture * lightningIcon;
	SDL_Texture * nullTexture;
	SDL_Texture * currentSlot;
	SDL_Texture * highlightedSlot;
	SDL_Texture * highlightedCurrent;
	SDL_Texture * chestDialog;
	SDL_Texture * chestDialogCloseButton;

	//fade to black
	void DrawFade();
	int blackness;
	int fadeTimePassed;
	int fadeTimeInMs;
	bool fading;
	bool fadeDirectionIn; //false: fade out    ---     true: fade in

};

#endif