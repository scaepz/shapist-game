#include "DialogHandler.h"
#include "SwitchWeapon.h"

CDialogHandler::CDialogHandler(CVectorManager * _vm, int resX, int resY)
{
	resolution[0] = resX;
	resolution[1] = resY;
	vm = _vm;
	inventory = new CInventoryOnScreen(vm->GetPlayer(), 48, 128);
	chestDialog = nullptr;
	mouseButtonDown = false;
	liftingChestIcon = false;
}

bool CDialogHandler::IsLifting(int index)
{
	if (index == iconInAirIndex && lifting)
		return true;
	else return false;
}

CDialogHandler::~CDialogHandler()
{
	delete inventory;
	if (chestDialog != nullptr) delete chestDialog;
}

CInventoryOnScreen *CDialogHandler::GetInventory() //can other objects do stuff with private members if they have the address? yes
{
	return inventory;
}

bool CDialogHandler::IsInventoryOpen()
{
	return inventory->IsOpen();
}

bool CDialogHandler::IsChestDialogOpen()
{
	if (chestDialog == nullptr)
	{
		return false;
	}
	else return true;
}
bool CDialogHandler::IsDialogOpen()
{
	return IsChestDialogOpen();
	//and other dialogs
}
void CDialogHandler::OpenChestDialog(CChest * _chest)
{
	SDL_Rect dialogRect;
	chest = _chest;
	dialogRect.w = resolution[0] / 6;
	if (dialogRect.w < 128)
	{
		dialogRect.w = 128;
	}
	dialogRect.h = dialogRect.w + dialogRect.w/7;
	dialogRect.x = playerPosOnScreen[0] + 48;
	dialogRect.y = playerPosOnScreen[1] - 48;

	if (dialogRect.x > resolution[0] - dialogRect.w)
	{
		dialogRect.x = resolution[0] - dialogRect.w;
	}
	if (dialogRect.y < 0)
	{
		dialogRect.y = 0;
	}
	else if (dialogRect.y > resolution[1] - dialogRect.h)
	{
		dialogRect.y = resolution[1] - dialogRect.h;
	}
	if (chestDialog != nullptr)
	{
		delete chestDialog;
		chestDialog = nullptr;
	}
	chestDialog = new CChestDialog(chest->GetContent(), chest->GetKey(), dialogRect); // one way to kill would be to send it its own ptr that it can set to nullptr
	inventory->OpenInventory();
}
void CDialogHandler::CloseInventory()
{
	inventory->CloseInventory();
	lifting = false;
	mouseButtonDown = false;
	iconInAirIndex = -1;
}
void CDialogHandler::CloseDialog()
{
	delete chestDialog;
	chestDialog = nullptr;
	CloseInventory();
}

void CDialogHandler::Click(int x, int y)
{

	mouseButtonDown = true;

	clickCoords[0] = x;
	clickCoords[1] = y;

}
void CDialogHandler::CloseChestDialog()
{
	if (chestDialog != nullptr)
	{
		delete chestDialog;
		chestDialog = nullptr;
		chest = nullptr;
		CloseInventory();
	}
}
void CDialogHandler::Release(int x, int y)
{
	mouseButtonDown = false;
	if (chestDialog != nullptr)
	{
		if (liftingChestIcon)
		{
			for (int i = 0; i < numberOfSlots; i++)//hardcoded for now, but size and pos of inv might vary with resolution
			{
				if (x > 48 + (128 * i) && x < 48 + 128 + (128 * i) && y > 0 && y < 128)
				{
					//swap slot and chest
					int tempWep = vm->GetPlayer()->WeaponAtSlot(i);
					vm->GetPlayer()->EmptySlot(i);
					vm->GetPlayer()->AddWeaponToSlot(chest->GetContent(), i);
					chest->SetContent(tempWep);
				}
			}
			liftingChestIcon = false;
			lifting = false;
		}
		else
		{
			//if click and release is inside close-rect
			if (clickCoords[0] > chestDialog->closeRect.x
				&& clickCoords[0] < chestDialog->closeRect.x + chestDialog->closeRect.w
				&& clickCoords[1] > chestDialog->closeRect.y
				&& clickCoords[1] < chestDialog->closeRect.y + chestDialog->closeRect.h)
			{
				if (x > chestDialog->closeRect.x
					&& x < chestDialog->closeRect.x + chestDialog->closeRect.w
					&& y > chestDialog->closeRect.y
					&& y < chestDialog->closeRect.y + chestDialog->closeRect.h)
				{
					CloseChestDialog();
				}
			}
		}
	}
	if (IsInventoryOpen())
	{
		if (!lifting)
		{
			// releasing mousekey on slot when not lifting equips the weapon in that slot
			for (int i = 0; i < numberOfSlots; i++)//hardcoded for now, but size and pos of inv might vary with resolution
			{
				if (x > 48 + (128 * i) && x < 48 + 128 + (128 * i) && y > 0 && y < 128)
				{
					CCommandObject * switchWep = new CSwitchWeapon(vm->GetPlayer(), i);
					vm->AddObject(switchWep);
				}
			}
		}
		else
		{	// releasing mouse key on slot when lifting swaps the slots
			for (int i = 0; i < numberOfSlots; i++)//hardcoded for now, but size and pos of inv might vary with resolution
			{
				if (x > 48 + (128 * i) && x < 48 + 128 + (128 * i) && y > 0 && y < 128)
				{
					vm->GetPlayer()->SwapTwoSlots(iconInAirIndex, i);
				}
			}
			// releasing mouse key on chest when lifting swaps the slots
			if (!liftingChestIcon && IsChestDialogOpen())
			{
				if (x > chestDialog->iconRect.x && x < chestDialog->iconRect.x + chestDialog->iconRect.w
					&& y > chestDialog->iconRect.y && y < chestDialog->iconRect.y + chestDialog->iconRect.h)
				{
					int tempWep = vm->GetPlayer()->WeaponAtSlot(iconInAirIndex);
					vm->GetPlayer()->EmptySlot(iconInAirIndex);
					vm->GetPlayer()->AddWeaponToSlot(chest->GetContent(), iconInAirIndex);
					chest->SetContent(tempWep);
				}
			}
			lifting = false;
			iconInAirIndex = -1;
		}
	}
}
bool CDialogHandler::IsHighLighted(int index)
{
		int x; int y;
		SDL_GetMouseState(&x, &y);
		if (x > 48 + (128 * index) && x < 48 + 128 + (128 * index) && y > 0 && y < 128)
		{
			return true;
		}
		else return false;
}
SDL_Rect CDialogHandler::GetSlotRect(int index)
{
	return inventory->GetRect(index);
}

SDL_Rect CDialogHandler::GetIconRect(int index)
{
	if (!IsLifting(index))
	{
		return inventory->GetRect(index);
	}
	else
	{
		SDL_Rect rect = inventory->GetRect(index);
		int x1; int y1;
		SDL_GetMouseState(&x1, &y1);
		rect.x = x1 - gripCoords[0];
		rect.y = y1 - gripCoords[1];
		return rect;
	}
}

SDL_Rect CDialogHandler::GetChestIconRect()
{
	if (!liftingChestIcon)
	{
		return chestDialog->iconRect;
	}
	else
	{
		SDL_Rect rect = chestDialog->iconRect;
		int x1; int y1;
		SDL_GetMouseState(&x1, &y1);
		rect.x = x1 - gripCoords[0];
		rect.y = y1 - gripCoords[1];
		return rect;
	}

}

void CDialogHandler::Update()
{
	if (mouseButtonDown && !lifting)
	{
		int x1; int y1;
		SDL_GetMouseState(&x1, &y1);
		if (sqrt((clickCoords[0] - x1)*(clickCoords[0] - x1) + (clickCoords[1] - y1)*(clickCoords[1] - y1)) > 8) //If mouse have traveled more than 8 pixels since click (it's pythagoras)
		{
			lifting = true;
			//set which icon is lifting
			if (chestDialog != nullptr)
			{
				if (clickCoords[0] > chestDialog->iconRect.x
					&& clickCoords[0] < chestDialog->iconRect.x + chestDialog->iconRect.w
					&& clickCoords[1] > chestDialog->iconRect.y
					&& clickCoords[1] < chestDialog->iconRect.y + chestDialog->iconRect.h)
				{
					gripCoords[0] = clickCoords[0] - chestDialog->iconRect.x;
					gripCoords[1] = clickCoords[1] - chestDialog->iconRect.y;
					liftingChestIcon = true;
					iconInAirIndex = -1;
					
				}
			}
			for (int i = 0; i < numberOfSlots; i++)
			{
				if (clickCoords[0] > 48 + (128 * i) && clickCoords[0] < 48 + 128 + (128 * i) && clickCoords[1] > 0 && clickCoords[1] < 128)
				{
					iconInAirIndex = i;
					gripCoords[0] = clickCoords[0] - (48 + (128 * i));
					gripCoords[1] = clickCoords[1];
				}
			}
		}
	}
}

SDL_Rect CDialogHandler::GetChestRect()
{
	return chestDialog->rect;
}
SDL_Rect CDialogHandler::GetChestCloseButtonRect()
{
	return chestDialog->closeRect;
}