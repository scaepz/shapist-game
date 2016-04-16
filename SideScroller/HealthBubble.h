#ifndef HEALTHBUBBLE_H
#define HEALTHBUBBLE_H
#include "Item.h"
class CHealthBubble :
	public CItem
{
public:
	CHealthBubble(int x, int y, int textureId, int size, int hp);
	CHealthBubble(int x, int y, int size, int hp);
	virtual ~CHealthBubble();
	void GetPickedUp(CPlayer * picker);

private:
	int hp;

};
#endif // !HEALTHBUBBLE_H

