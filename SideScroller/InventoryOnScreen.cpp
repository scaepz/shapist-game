#include "InventoryOnScreen.h"


CInventoryOnScreen::CInventoryOnScreen(CPlayer * _player, int x, int slotsize)
{
	firstSlotPosition = x;
	slotSize = slotsize;
	player = _player;
	amountOfSlots = numberOfSlots;
	msLeft = 0;
	animationTimeInMs = 70;
	isOpen = false;
	isOpening = false;
	isClosing = false;
}

SDL_Rect CInventoryOnScreen::GetRect(int index)
{
	SDL_Rect rect;
	rect.x = firstSlotPosition + slotSize*index;
	rect.y = -slotSize + (slotSize* GetInventoryVerticalPositionPercent());
	rect.w = slotSize;
	rect.h = slotSize;
	return rect;
}

void CInventoryOnScreen::OpenInventory()
{
	if (!IsOpen())
	{
		isClosing = false;
		isOpening = true;
		msLeft = 0;
	}
}
void CInventoryOnScreen::CloseInventory()
{
	isOpening = false;
	isOpen = false;
	isClosing = true;
	msLeft = animationTimeInMs;
}

CInventoryOnScreen::~CInventoryOnScreen()
{
}
int CInventoryOnScreen::GetSlotWeapon(int slotIndex)
{
	return player->WeaponAtSlot(slotIndex);
}


float CInventoryOnScreen::GetInventoryVerticalPositionPercent()
{
	float percentDown;
	percentDown = (float)msLeft / (float)animationTimeInMs;
	return percentDown;

}

bool CInventoryOnScreen::IsOpen() //returns true if open or opening. Artist should also know when its closing, but the cursor and input change should disappear as soon as key is released
{
	if (isOpen || isOpening)
	{
		return true;
	}
	else return false;
}

void CInventoryOnScreen::Update()
{
	if (isClosing)
	{

		if (msLeft > 0)
		{
			msLeft -= g_time;
		}
		else
		{
			msLeft = 0;
			isClosing = false;
		}
	}
	else if (isOpening)
	{
		if (msLeft < animationTimeInMs)
		{
			msLeft += g_time;
		}
		else
		{
			msLeft = animationTimeInMs;
			isOpening = false;
			isOpen = true;
		}
	}
}