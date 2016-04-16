#include "Weapon.h"


CWeapon::CWeapon()
{
	flipPoint.x = 0;
	flipPoint.y = 0;
	animPointOfNoReturnInMs = -1;
	positionAdjustment[0] = 0;
	positionAdjustment[1] = 0;
	idlePositionAdjustment[0] = 0;
	idlePositionAdjustment[1] = 8;
	idleRotation = 15;
}


CWeapon::~CWeapon()
{

}

void CWeapon::CeaseFire()
{
	return;
}

void CWeapon::Update()
{

}
int CWeapon::GetFrame(int ms, int anim)
{
	return 0;
}
int CWeapon::GetFrameSizeX()
{
	return frameSizeX;
}
int CWeapon::GetFrameSizeY()
{
	return frameSizeY;
}