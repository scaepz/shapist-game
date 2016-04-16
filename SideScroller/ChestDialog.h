#ifndef CHESTDIALOG_H
#define CHESTDIALOG_H
#include "HudDialog.h"
#include "VectorManager.h"
class CChestDialog
{
public:
	CChestDialog(int weapon, int key, SDL_Rect rect);
	virtual ~CChestDialog();
//	void LiftWeapon();
	//void SwapWeapon(int newWeapon);
	//void 
	SDL_Rect rect;
	SDL_Rect iconRect;
	SDL_Rect closeRect;
private:
	int weapon;
	int key;
	
};

#endif