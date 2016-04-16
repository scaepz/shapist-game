#ifndef DIALOGHANDLER_H
#define DIALOGHANDLER_H
#include "InventoryOnScreen.h"
#include "VectorManager.h"
#include "HudDialog.h"
#include "ChestDialog.h"
#include "Chest.h"
//can currently only handle one dialog and one inventory at a time. might have to completely rewrite in the future.

class CDialogHandler
{
public:
	CDialogHandler(CVectorManager * vm, int resX, int resY);
	virtual ~CDialogHandler();
	CInventoryOnScreen * GetInventory();
	CHudDialog * GetDialog();
	
	void Update();
	void Click(int x, int y); //m1 click
	void Release(int x, int y); //m1 release

	void OpenChestDialog(CChest * _chest);
	void CloseChestDialog();
	SDL_Rect GetChestIconRect();
	SDL_Rect GetChestRect();
	SDL_Rect GetChestCloseButtonRect();
	bool IsChestDialogOpen();
	void CloseDialog();
	CChest * chest;
	bool IsDialogOpen();

	bool IsInventoryOpen();
	void CloseInventory();
	bool IsHighLighted(int index);
	SDL_Rect GetSlotRect(int index);
	SDL_Rect GetIconRect(int index);
private:
	bool mouseButtonDown;
	bool lifting;
	int iconInAirIndex;
	SDL_Rect liftRect;
	bool IsLifting(int index);
	int clickCoords[2];
	int gripCoords[2];
	int releaseCoords[2];
	CVectorManager * vm;

	
	CChestDialog * chestDialog = nullptr;
	int resolution[2];
	CInventoryOnScreen * inventory = nullptr;
	bool liftingChestIcon;


};

#endif