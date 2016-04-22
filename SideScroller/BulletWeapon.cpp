#include "BulletWeapon.h"
#include "Global.h"
#include "SDL.h"
#include <iostream>

CBulletWeapon::CBulletWeapon(CVectorManager * _vm)
{
	vm = _vm;

	textureId = 10;
	iconId = 8;
	offsetY = 10;
	offsetX = 65;
	width = 65;
	height = 20;

	flipPoint.x = 0;
	flipPoint.y = 10;

	reloadDelay = 1500;
	clipSize = 8;
	fireDelay = 500;
	switchDelay = 500;
	kickBackX = -3;
	kickBackY = -4;
	kickBackRot = -14;
	kickBackTime = 250;

	tracerIntensity = 0.5f;
	maxLength = 1000;
	numberOfPellets = 1;
	fireConeTightness = 100000.0f;
	damage = 6;
	semiAutomatic = false;
	recoilIncreasePerBullet = 0;
	spreadIncreasePerBullet = 0;
	maxExtraRecoil = 0.2f;
	maxExtraSpread = 30000.0f;
}


CBulletWeapon::~CBulletWeapon()
{
}
void CBulletWeapon::Update()
{
	//update (decrease) recoil and spread
	if (currentExtraRecoil > maxExtraRecoil)
	{
		currentExtraRecoil = maxExtraRecoil;
		currentExtraRecoil -= (float)g_time * 0.0004f;
	}
	else if (currentExtraRecoil > 0)
	{
		currentExtraRecoil -= (float)g_time * 0.0004f;
	}
	if (currentExtraRecoil < 0)
	{
		currentExtraRecoil = 0;
	}

	if (currentExtraSpread > maxExtraSpread)
	{
		currentExtraSpread = maxExtraSpread;
		currentExtraSpread -= g_time * 35;
	}
	else if (currentExtraSpread > 0)
	{
		currentExtraSpread -= g_time * 35;
	}
	if (currentExtraSpread < 0)
	{
		currentExtraSpread = 0;
	}




}
bool CBulletWeapon::Attack(int x, int y, float angle, CBaseObject* attacker)
{
	totalDamage = 0;
	bool playerHitEnemy = true;
	float radius = sqrt(offsetX*offsetX + offsetY*offsetY);

	float correctedOffsetX = (float)(radius * cos(angle));
	float correctedOffsetY = (float)(radius * sin(angle));

	int tileSize = vm->standardTileSize;
	vector<vector<CTile*>>* tileVector = vm->GetTileVector();
	//calculate bullet paths
	for (int i = 0; i < numberOfPellets; i++)
	{
		srand(rand());
		float newAngle = angle + (float)((rand() % 10000) / (fireConeTightness - currentExtraSpread)) - (10000 / (fireConeTightness - currentExtraSpread) / 2);



		if (newAngle > -1.5 && newAngle < 1.5)
		{
			newAngle -= currentExtraRecoil;
		}
		else
		{
			newAngle += currentExtraRecoil;
		}

		int x1 = x + correctedOffsetX;
		int y1 = y + correctedOffsetY;

		int x2 = x1 + maxLength * cos(newAngle);
		int y2 = y1 + maxLength * sin(newAngle);

		if (vm->GetTileVector()->at(y1 / vm->standardTileSize).at(x1 / vm->standardTileSize) != nullptr)
		{
			x2 = x1;
			y2 = y1;
		}

		float m = (float)((float)(y2 - y1) / (float)(x2 - x1)); //riktningskoefficient / slope
		//loopstuff

		int xAddition = 1;
		int yAddition = 1;

		int xBorder = 0;
		int yBorder = 0;

		if (x2 < x1)
		{
			xAddition = -1;
			xBorder = tileSize - 1;
		}
		if (y2 < y1)
		{
			yAddition = -1;
			yBorder = tileSize - 1;
		}

		float yIterator = y1;
		if (abs(m) < 1.5f)
		{
			for (int xIterator = x1; xIterator != x2; xIterator += xAddition)
			{
				yIterator += m * xAddition; //when x increases with 1, y increases with m

				if (xIterator / tileSize >= 0 && xIterator / tileSize < vm->GetTileVector()->at(0).size() && yIterator / tileSize >= 0 && yIterator / tileSize < vm->GetTileVector()->size()) //outside of map check
				{
					if (tileVector->at(yIterator / tileSize).at(xIterator / tileSize) != nullptr)
					{
						x2 = xIterator;
						y2 = yIterator;
						break;
					}
				}

			}
		}
		else
		{
			float xIterator = x1;
			m = (float)((float)(x2 - x1) / (float)(y2 - y1));
			for (int yIterator = y1; yIterator != y2; yIterator += yAddition)
			{
				xIterator += m * yAddition; //when y increases with 1, x increases with m

				if (xIterator / tileSize >= 0 && xIterator / tileSize < vm->GetTileVector()->at(0).size() && yIterator / tileSize >= 0 && yIterator / tileSize < vm->GetTileVector()->size())//outside of map check
				{
					if (tileVector->at(yIterator / tileSize).at(xIterator / tileSize) != nullptr)
					{
						x2 = xIterator;
						y2 = yIterator;
						break;
					}
				}

			}
		}
		int closestSolid = -1;
		for (int p = 0; p < vm->GetSolidVector()->size(); p++)
		{
			SDL_Rect enemy;
			enemy.x = vm->GetSolidVector()->at(p)->GetX();
			enemy.y = vm->GetSolidVector()->at(p)->GetY();
			enemy.w = vm->GetSolidVector()->at(p)->GetWidth();
			enemy.h = vm->GetSolidVector()->at(p)->GetHeight();
			int tempx1 = x1;
			int tempx2 = x2;
			int tempy1 = y1;
			int tempy2 = y2;
			if (SDL_IntersectRectAndLine(&enemy, &tempx1, &tempy1, &tempx2, &tempy2))
			{
				x2 = tempx1;
				y2 = tempy1;
				closestSolid = p;
			}

		}
		if (closestSolid != -1)
		{
			if (attacker != vm->GetPlayer() && vm->GetSolidVector()->at(closestSolid) == vm->GetPlayer())
			{
				vm->GetSolidVector()->at(closestSolid)->TakeDamage(damage);
			}
			else if (attacker == vm->GetPlayer() && vm->GetSolidVector()->at(closestSolid) != vm->GetPlayer())
			{
				totalDamage += damage;
				CDamageNumber * dmgn = new CDamageNumber(damage, x2, y2);
				playerHitEnemy = true;
				vm->AddObject(dmgn);
				vm->GetSolidVector()->at(closestSolid)->TakeDamage(damage);
			}

		}
		vm->GetParticleEngine()->NewBulletTracer(x1, y1, x2, y2, tracerIntensity);
		currentExtraRecoil += recoilIncreasePerBullet;
		currentExtraSpread += spreadIncreasePerBullet;
	}
	if (totalDamage != 0)
	{
		if (playerHitEnemy)
			vm->GetSoundPlayer()->PlayDamageSound(totalDamage);
	}
	if (fireSound != -1)
	{
		vm->GetSoundPlayer()->PlaySound(fireSound, 0);
	}
	return semiAutomatic;
}