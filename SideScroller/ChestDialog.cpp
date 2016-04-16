#include "ChestDialog.h"
#include <map>

CChestDialog::CChestDialog(int _weapon, int _key, SDL_Rect _rect)
{
	weapon = _weapon;
	key = _key;
	rect = _rect;

	iconRect.w = 0.8f*rect.w;
	iconRect.h = 0.6f*rect.h;
	iconRect.x = rect.x + rect.w / 10;
	iconRect.y = rect.y + rect.h / 8;

	closeRect.w = rect.w / 2;
	closeRect.h = closeRect.w * 0.3;
	closeRect.x = rect.x + rect.w / 4;
	closeRect.y = rect.y + rect.h - closeRect.h - closeRect.h / 2;
}



CChestDialog::~CChestDialog()
{
}
