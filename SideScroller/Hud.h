#ifndef HUD_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <SDL_image.h>
#include "DamageNumber.h"
#include "VectorManager.h"
class CHud 
{
public:
	//storage and updater for hud textures
	// to include dmg numbers, inventory, dialog? probably yes but not sure as of now
	CHud(SDL_Renderer *, CVectorManager * vm, int resX, int resY);
	virtual ~CHud();
	void Update();
	
	//stamina
	SDL_Rect GetStaminaOuterBorderRect();
	SDL_Rect GetStaminaInnerBorderRect();
	SDL_Rect GetStaminaBarRect();
	int GetStaminaRedness();

	//Gives pointer address to next damage number texture and fills the rect with size and _absolute_ dest, returns false if there are no more this "loop"
	bool GetNextDamageNumber(SDL_Texture * & texturePointer, SDL_Rect & dest, int & alpha); 

	//hp
	SDL_Texture * GetHpTexture();
	SDL_Rect GetHpRect();

	//ammo
	SDL_Texture * GetClipTexture();
	SDL_Texture * GetReserveAmmoTexture();
	SDL_Rect GetClipRect();
	SDL_Rect GetReserveAmmoRect();

private:
	SDL_Renderer * renderer;
	SDL_Color hudColor;

	//damage numbers
	std::map<CDamageNumber*, SDL_Texture*> damageNumberTextureMap; //map each damage number object to a texture, so it's simple to destroy both at the same time
	void UpdateDamageNumbers();
	TTF_Font * dmgNumberFont;
	int damageNumberIndex;
	int damageNumberPixels[2];

	//stamina bar
	SDL_Rect staminaInnerBorderRect;
	SDL_Rect staminaOuterBorderRect;
	SDL_Rect staminaBarRect;
	int staminaBarMaxWidth;

	//hp
	TTF_Font * hpFont;
	int lastHp;
	SDL_Texture * hpTexture;
	void UpdateHp();
	
	SDL_Rect hpRect;
	int hpNumberPixels[2];
	
	//ammo
	int lastClip;
	int lastReserve;
	SDL_Texture * clipTexture;
	SDL_Texture * reserveTexture;
	SDL_Rect clipRect;
	SDL_Rect reserveRect;
	void UpdateAmmo();
	int reserveAmmoPixels[2];


	int resolution[2];
	CVectorManager * vm;

	
};

#define HUD_H
#endif // !HUD_H
