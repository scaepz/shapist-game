#include "MeleeWeapon.h"
#include "Global.h"
#include <iostream>

CMeleeWeapon::CMeleeWeapon(CVectorManager * _vm)
{
	vm = _vm;
	textureId = 106;
	iconId = 404;
	animationData[idle][frameCountData] = 9;
	animationData[idle][msPerFrameData] = 700;
	animationData[idle][startFrameData] = 0;
	animationData[attack1][frameCountData] = 7;
	animationData[attack1][msPerFrameData] = 25;
	animationData[attack1][startFrameData] = 9;
	animationData[attack2][frameCountData] = 8;
	animationData[attack2][msPerFrameData] = 25;
	animationData[attack2][startFrameData] = 16;
	fireDelay = 200;
	flipPoint.x = 0;
	flipPoint.y = 10;
	currentAnimation = 0;
	timeSinceAnimationStart = 0;
	frameSizeX = 72;
	frameSizeY = 20;
	height = 20;
	width = 72;
	StartAnimation(idle);
	maxLength = 28;
	offsetX = 48;
	offsetY = 0;
	damage = 3;
	ammoType = melee;
	ammoCost = 0;
	staminaCost = 8;

}

bool CMeleeWeapon::Attack(int x, int y, float angle)
{
	//float angle = atan2(dirY, dirX);

	float radius = sqrt(offsetX*offsetX + offsetY*offsetY);

	float correctedOffsetX = (float)(radius * cos(angle));
	float correctedOffsetY = (float)(radius * sin(angle));

	int x1 = x + correctedOffsetX;
	int y1 = y + correctedOffsetY;

	int x2 = x1 + maxLength * cos(angle);
	int y2 = y1 + maxLength * sin(angle);

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
	if (closestSolid > -1)
	{
		vm->GetSolidVector()->at(closestSolid)->TakeDamage(damage);
		vm->GetSoundPlayer()->PlayDamageSound(damage);
		CDamageNumber * dmgn = new CDamageNumber(damage, x2, y2);
		vm->AddObject(dmgn);
	}

	return true;
}
CMeleeWeapon::~CMeleeWeapon()
{
}

int CMeleeWeapon::GetFrame(int ms, int anim)
{
	if (anim < 0 || anim > numberOfAnimations) return -1;
	if (animationData[anim][frameCountData] == 0) return -1;
	if (ms / animationData[anim][1] < animationData[anim][frameCountData])
	{
		return animationData[anim][startFrameData] + ms / animationData[anim][msPerFrameData];
	}
	else
	{
		return -1;
	}
}

void CMeleeWeapon::Update()
{
	timeSinceAnimationStart += g_time;

}
void CMeleeWeapon::StartAnimation(int i)
{
	if (i >= 0 && i < numberOfAnimations)
		currentAnimation = i;
	else currentAnimation = 0;
	timeSinceAnimationStart = 0;
}