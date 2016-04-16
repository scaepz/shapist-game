#include "HitscanWeapon.h"
#include <iostream>
#include "Global.h"

#include <iostream>
#include "SDL.h"

CHitscanWeapon::CHitscanWeapon(CVectorManager *_vm)
{
	
	vm = _vm;
	tileVector = vm->GetTileVector();
	baseVector = vm->GetBaseVector();
	solidVector = vm->GetSolidVector();
	fireDelay = 0;
	clipSize = INT_MAX;
	reloadDelay = 0;
	switchDelay = 300;
	offsetX = 50;
	offsetY = 5;
	flipPoint.x = 0;
	flipPoint.y = 10;
	width = 50;
	height = 20;
	textureId = 9;
	iconId = 4004;
	kickBackX = 0;
	kickBackY = 0;
	kickBackRot = 0;
	kickBackTime = 0;
	msBetweenDmg = 100;
	msSinceDmg = msBetweenDmg;
}


CHitscanWeapon::~CHitscanWeapon()
{
}

void CHitscanWeapon::UpdateTime()
{
	msSinceDmg += g_time;
}

bool CHitscanWeapon::Attack(int x, int y, float dirX, float dirY)
{
	
	if (debugPause)
	{
		abs(1);
	}
	////////////
	//This method creates a collection of CDevLasers and puts them into basevector
	//The objects stay as long as needed and get their pos and rotation updated each time the method is called
	//
	bool breaker = false;
	//rotation amount of textures
	float degrees = atan2(dirY, dirX);
	degrees *= (180 / 3.1415);

	//calculate x and y amount for next piece in beam

	float a = sqrt(dirX*dirX + dirY*dirY) / (4 - 1); //length of piece of beam -1 (-1 because otherwise we get small gaps at some angles)
	float speedX = dirX / a;
	float speedY = dirY / a;

	//calculate starting position. starting position is somewhere in a circle around the object. Circle size is given by weapon size, which is given by offsetX and offsetY
	//we then add these coordinates to the objects center coordinates
	float angle = atan2(dirY, dirX);
	float radius = sqrt(offsetX*offsetX + offsetY*offsetY);

	//find the point on the circle
	float correctedOffsetX = (float)(radius * cos(angle));
	float correctedOffsetY = (float)(radius * sin(angle));

	//temp holder for next x and y pos, and counter
	int temp[2] = { 0 };
	int i = -1;
	//bool breaker = false;
	do
	{	//calculate next pos
		i++;
		temp[0] = x+correctedOffsetX + i*(speedX);
		temp[1] = y+correctedOffsetY + i*(speedY);

		//check if next pos is outside map, if so break
		if (temp[0] / 64 > tileVector->at(1).size() - 1 || temp[0] / 64 < 0 || temp[1] / 64 > tileVector->size() - 1 || temp[1] / 64 < 0)
		{
			break;
		}
		//if i >= laservector.size that means that the beam in this frame is longer than last frames, and we need new objects
		if (i >= laserVector.size())
		{
			//we push it into basevector for rendering purposes
			CDevLaser * laserPiece = new CDevLaser(temp[0], temp[1], 2, 4, 4, degrees);
			laserVector.push_back(laserPiece);
		//	vm->AddBackgroundObject(laserPiece);

		}
		else //else we just update the old objects
		{
			laserVector[i]->pos[0] = temp[0];
			laserVector[i]->pos[1] = temp[1];
			laserVector[i]->rotation = degrees;
		}

		//check if it collides with any damagables
		for (int i = 0; i < solidVector->size(); i++)
		{

			SDL_Rect obj;
			obj.x = solidVector->at(i)->GetX();
			obj.y = solidVector->at(i)->GetY();
			obj.w = solidVector->at(i)->GetWidth();
			obj.h = solidVector->at(i)->GetHeight();


			SDL_Point intersection;
			int  x1;
			x1 = laserVector[0]->GetX();
			int  x2;
			x2 = laserVector[laserVector.size() - 1]->GetX() + speedX;
			int  y1;
			y1 = laserVector[0]->GetY() + (laserVector[0]->GetHeight() / 2);
			int  y2;
			y2 = laserVector[laserVector.size() - 1]->GetY() + (laserVector[0]->GetHeight() / 2) + speedY;

			if (SDL_IntersectRectAndLine(&obj, &x1, &y1, &x2, &y2))
			{
				for (int a = 0; a < msSinceDmg / msBetweenDmg; a++)
				{
					int damage = 2;
					solidVector->at(i)->TakeDamage(damage);
					CDamageNumber* dmg = new CDamageNumber(damage, solidVector->at(i)->GetX(), solidVector->at(i)->GetY());
					vm->GetSoundPlayer()->PlayDamageSound(damage);
					vm->AddObject(dmg);
					msSinceDmg -= msBetweenDmg;
					a--;
				}
				breaker = true;
				break;
			}
		}

		//rotated rectangle collision detection

	} while (tileVector->at(temp[1] / 64).at(temp[0] / 64) == nullptr && breaker == false); //loop this as long as no laserobject is on top of a tile

	//i only grows as large as needed. when a laserobject is on top of a tile, the loop ends and i stops growing
	//the point where i stopped growing is the point where the beam ended. if laservector is bigger than i then there are too many objects, so we delete.
	while (i < laserVector.size())
	{
		for (int o = baseVector->size() - 1; o >= 0; o--)
		{
			//delete object and its pointers
			if (baseVector->at(o) == laserVector.at(i))
			{
				vm->DeleteObject(baseVector->at(o));
				laserVector.erase(laserVector.begin() + i, laserVector.begin() + i + 1);
				break;
			}

		}
	}


	//return false means that the attack command will not be deleted. in this case, the attack command should be deleted by other means,
	//for example:
	//1. the user lets go of the attack button (inputhandler deletes it)
	//2. out of ammo (CAnimate returns true -> commandhandler deletes it)
	return false;
}

void CHitscanWeapon::CeaseFire()
{
	//delete all objects in vector
	//sucks if two animates use this weapon...
	//problem for the future
	while (laserVector.size() > 0)
	{
		for (int i = baseVector->size()-1; i >= 0; i--)
		{
			if (laserVector.at(0) == baseVector->at(i))
			{
				delete baseVector->at(i);
				baseVector->erase(baseVector->begin() + i);
				laserVector.erase(laserVector.begin());
				break;
			}
		}
	}
}