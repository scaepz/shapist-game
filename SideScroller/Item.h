#ifndef ITEM_H
#define ITEM_H

#include "PhysicsObject.h"
#include "Player.h"
class CItem :
	public CPhysicsObject
{
public:
	CItem(int x, int y, int textureId, int size);
	virtual ~CItem();
	virtual void GetPickedUp(CPlayer * picker) = 0;
	int GetFrame();
	void OnCollision(bool, bool);
private:
	int randSeed;
};


#endif // !ITEM_H
