#include "Enemy.h"
#include <string>

CEnemy::CEnemy(int x, int y, int textureId, int size, vector<CWeapon*> & weaponVector) : CAnimate(x, y, textureId, size, weaponVector)
{
	hitbox = square;
	
	SetWeaponEquipped(_spear_mudhands);
	timeSincePlayerVisibilityCheck = 0;
	timeSincePlayerNotVisible = 0;
	reactionTime = 230;
	crosshairVelocity = 0;
	SetPointingDirection(1, 1);
}
float CEnemy::GetXhairAngleDeg()
{
	if (currentActivity == CurrentActivity::idling)
	{
		if (lookingLeft) return 180;
		else return 0;
	}
	else return CAnimate::GetXhairAngleDeg();
}

void CEnemy::ResetTimeUntilReaction()
{
	timeUntilReaction = reactionTime;
}
int CEnemy::GetTimeUntilReaction()
{
	return timeUntilReaction;
}

void CEnemy::UpdateTimeUntilReaction(int time)
{
	timeUntilReaction -= time;
}
void CEnemy::SetAwareOfPlayer(bool a)
{
	awareOfPlayer = a;
}
CEnemy::~CEnemy()
{
}

//int CEnemy::GetFrame()
//{
//	return 0;
//}
void CEnemy::OnCollision(bool stopHor, bool stopVer)
{
	if (stopHor)
	{
		velocity.SetX(velocity[0]*-0.5);
		
	}
	if (stopVer)
	{
		velocity.SetY(0);
	}
}
void CEnemy::SetRotation(float _rotation)
{
	rotation = _rotation;
}



bool CEnemy::GetPlayerVisible()
{
	return playerVisible;
}
bool CEnemy::GetPlayerVisibleStale()
{
	return (timeSincePlayerVisibilityCheck > 300);
}
void CEnemy::SetPlayerVisible(bool newPlayerVisibility)
{
	
	if (playerVisible && newPlayerVisibility == false)
	{
		timeSincePlayerNotVisible = 0;
	}

	playerVisible = newPlayerVisibility;
	timeSincePlayerVisibilityCheck = 0;
}

void CEnemy::UpdateTime(int time)
{

	CAnimate::UpdateTime(time);
	timeSincePlayerVisibilityCheck += time;
	AITimer += time;
	if (!GetPlayerVisible())
	{
		timeSincePlayerNotVisible += time;
	}
}
bool CEnemy::IsAwareOfPlayer()
{
	return awareOfPlayer;
	if (GetPlayerVisible())
	{
		return true;
	}
	else if (timeSincePlayerNotVisible < 3000)
	{
		return true;
	}
	else return false;
}