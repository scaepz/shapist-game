#ifndef AMMOITEM_H
#define AMMOITEM_H
#include "Item.h"
class CAmmoItem :
	public CItem
{
public:
	CAmmoItem(int x, int y, int textureId, int size, int _ammoType, int _ammoValue);
	virtual ~CAmmoItem();
	virtual void GetPickedUp(CPlayer * picker);
private:
	int ammoType;
	int ammoValue;
};

#endif