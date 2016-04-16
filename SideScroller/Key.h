#ifndef KEY_H
#define KEY_H
#include "Item.h"
class CKey :
	public CItem
{
public:
	CKey(int x, int y, int textureId, int color);
	int color;
	void GetPickedUp(CPlayer * picker);

	virtual ~CKey();
};

#endif