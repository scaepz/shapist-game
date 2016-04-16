#include "CeaseFire.h"
#include "Weapon.h"

CCeaseFire::CCeaseFire(CAnimate * obj) : CCommandObject(obj)
{
	commandId = ceaseFire;
}


CCeaseFire::~CCeaseFire()
{
}

bool CCeaseFire::Execute()
{
	return obj->CancelAttack();	
}