#include "SwitchWeapon.h"


CSwitchWeapon::CSwitchWeapon(CPlayer * obj, int _slot) : CCommandObject(obj)
{
	player = obj;
	slot = _slot; 
	commandId = switchWeapon;
}


CSwitchWeapon::~CSwitchWeapon()
{
}

bool CSwitchWeapon::Execute()
{
	player->SwitchWeapon(slot);
	return true;
}