#include "RocketLauncher.h"


CRocketLauncher::CRocketLauncher(CVectorManager * _vm)
{
	vm = _vm;
	projectileSpeed = 0.7f;
	textureId = 10;
	iconId = 8;
	offsetY = 10;
	offsetX = 65;
	width = 65;
	height = 20;


	flipPoint.x = 0;
	flipPoint.y = 10;

	reloadDelay = 2000;
	clipSize = 1;
	fireDelay = 100;
	switchDelay = 500;
	kickBackX = -3;
	kickBackY = -3;
	kickBackRot = -10;
	kickBackTime = 250;
}


CRocketLauncher::~CRocketLauncher()
{
}


bool CRocketLauncher::Attack(int x, int y, float angle)
{

	//calculate starting x and y speed
	float speedX = projectileSpeed * cos(angle);
	float speedY = projectileSpeed * sin(angle);

	//calculate starting position. starting position is somewhere in a circle around the object. Circle size is given by weapon size, which is given by offsetX and offsetY
	//we then add these coordinates to the objects center coordinates
	//float angle = atan2(dirY, dirX);
	float radius = sqrt(offsetX*offsetX + offsetY*offsetY);

	//find the point on the circle
	float correctedOffsetX = (float)(radius * cos(angle));
	float correctedOffsetY = (float)(radius * sin(angle));

	int rocketWidth = 32;
	int rocketHeight = 10;

	int xPos = x + correctedOffsetX + (rocketWidth * (correctedOffsetX / abs(correctedOffsetX)) / 2);
	int yPos = y + correctedOffsetY + (rocketHeight * (correctedOffsetY / abs(correctedOffsetY)) / 2);
	//create the grenade and insert into neccessary vectors
	CRocket * rocket = new CRocket(xPos, y + correctedOffsetY + rocketHeight / 302, 1002, 44, 10, 8, 150, rocketWidth, rocketHeight, speedX, speedY, true, 10000, 400, 1, 0);
	rocket->SetRotation(angle*(180 / 3.14f));
	vm->AddObject(rocket);


	return true;

}