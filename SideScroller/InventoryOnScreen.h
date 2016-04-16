#ifndef INVENTORYONSCREEN_H
#define INVENTORYONSCREEN_H
#include "Global.h"
#include "Player.h"
class CInventoryOnScreen
{
public:
	CInventoryOnScreen(CPlayer * _player, int x, int size);
	virtual ~CInventoryOnScreen();
	int GetSlotWeapon(int slotIndex);
	void OpenInventory();
	void CloseInventory();
	float GetInventoryVerticalPositionPercent();
	void Update();
	bool IsOpen();
	SDL_Rect GetRect(int index);
private:
	CPlayer * player;
	bool isClosing;
	bool isOpening;
	bool isOpen;
	int msLeft; //goes both ways depending on closing or opening
	int firstSlotPosition; //x
	int slotSize;
	int amountOfSlots;
	int animationTimeInMs; //how long it takes for the inventory to roll down
};

#endif