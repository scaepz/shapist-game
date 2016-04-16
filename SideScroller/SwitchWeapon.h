#ifndef SWITCHWEAPON_H
#define SWITCHWEAPON_H
#include "Player.h"
#include "CommandObject.h"
class CSwitchWeapon :
	public CCommandObject
{
public:
	CSwitchWeapon(CPlayer * obj, int slot);
	virtual ~CSwitchWeapon();
	bool Execute();
private:
	CPlayer * player;
	int slot;
};

#endif