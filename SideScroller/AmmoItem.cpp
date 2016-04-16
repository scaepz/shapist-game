#include "AmmoItem.h"


CAmmoItem::CAmmoItem(int x, int y, int textureId, int size, int _ammoType, int _ammoValue) : CItem(x,y,textureId,size)
{
	ammoType = _ammoType;
	ammoValue = _ammoValue;
}


CAmmoItem::~CAmmoItem()
{
}
void CAmmoItem::GetPickedUp(CPlayer * picker)
{
	picker->AddAmmo(ammoType, ammoValue);
}
