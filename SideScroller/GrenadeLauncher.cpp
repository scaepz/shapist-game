#include "GrenadeLauncher.h"
#include "SoundEnum.h"

CGrenadeLauncher::CGrenadeLauncher(CVectorManager * _vm)
{
	vm = _vm;
	projectileSpeed = 0.9;
	textureId = 103;
	iconId = 203;
	offsetY = 10;
	offsetX = 65;
	width = 65;
	height = 20;


	flipPoint.x = 0;
	flipPoint.y = 10;

	reloadDelay = 1000;
	clipSize = 2;
	fireDelay = 100;
	switchDelay = 500;
	kickBackX = -3;
	kickBackY = -3;
	kickBackRot = -10;
	kickBackTime = 250;
	ammoType = explosiveAmmo;
	ammoCost = 2;
}


CGrenadeLauncher::~CGrenadeLauncher()
{
}

bool CGrenadeLauncher::Attack(int x, int y, float angle, CBaseObject* attacker)
{
	float speedX = projectileSpeed * cos(angle);
	float speedY = projectileSpeed * sin(angle);

	//calculate starting position. starting position is somewhere in a circle around the object. Circle size is given by weapon size, which is given by offsetX and offsetY
	//we then add these coordinates to the objects center coordinates
//	float angle = atan2(dirY, dirX);
	float radius = sqrt(offsetX*offsetX + offsetY*offsetY);

	//find the point on the circle
	float correctedOffsetX = (float)(radius * cos(angle));
	float correctedOffsetY = (float)(radius * sin(angle));
	correctedOffsetX *= 2;
	correctedOffsetY *= 2;

	//create the grenade and insert into neccessary vectors
	CGrenade * grenade = new CGrenade(x + correctedOffsetX, y + correctedOffsetY, 301, 12, 12, 1,1000,12, speedX, speedY, true, 1000, 300, 1, 120);
	vm->GetSoundPlayer()->PlaySound(grenadeLauncher_fire, 0);
	vm->AddObject(grenade);


	return true;
}