#include "Enemy.h"
#include "Global.h"

CEnemy::CEnemy(int x, int y, int textureId, int size, vector<CWeapon*> & weaponVector) : CAnimate(x, y, textureId, size, weaponVector)
{
	hitbox = square;

	SetWeaponEquipped(_spear_mudhands);
	timeSincePlayerVisibilityCheck = 0;
	timeSincePlayerNotVisible = 0;
	xhairVelocity = 0;
	SetPointingDirection(1, 1);
	speechType = "generic";
	AddAmmo(bulletAmmo, 200);
	for (int i = 0; i < numberOfWeapons; i++)
	{
		ammoInClip[i] = weaponVector[i]->clipSize;
	}
	currentActivity = CurrentActivity::idling;
}

void CEnemy::TakeDamage(int damage)
{
	tookDamage = true; //for AI player awareness
	CAnimate::TakeDamage(damage);
}
float CEnemy::GetXhairAngleDeg()
{
	if (IsAwareOfPlayer() == false)
	{
		if (lookingLeft) return 180;
		else return 0;
	}
	 return CAnimate::GetXhairAngleDeg();
}

void CEnemy::Aim()
{
	float distance = GetXhairAngle() - optimalAngle + xhairAngleRandomAddition;
	if (distance == 0) return;
	int direction = (distance / abs(distance)) * -1;
	if (abs(distance) > 3.1415f)
	{
		if (direction == 1) distance += 3.1415f;
		else
		{
			distance -= 3.1415f;
		}
		direction *= -1;
	}

	xhairVelocity = abs(distance) * direction * 0.005f;

	SetXhairAngle(GetXhairAngle() + xhairVelocity*g_time);
}
void CEnemy::SetOptimalXhairAngle(float angle)
{
	timeUntilAimCorrection = (rand() % 300) + 50;
	optimalAngle = angle;
}
pugi::char_t * CEnemy::GetActivityString()
{
	switch (currentActivity)
	{
	case CurrentActivity::idling:
		return "idling";
	case CurrentActivity::deserting:
		return "deserting";
	case CurrentActivity::alert:
		return "alert";
	case CurrentActivity::fighting:
		return "fighting";
	case CurrentActivity::retreating:
		return "retreating";
	case CurrentActivity::braveryboost:
		return "braveryboost";
	case CurrentActivity::spottedPlayer:
		return "spotted";
	case CurrentActivity::searching:
		return "searching";
	default:
		return "";

	}
}

void CEnemy::React()
{
	timeUntilReaction = REACTION_TIME;
	isReacting = true;
}
bool CEnemy::IsReacting()
{
	return isReacting;
}
bool CEnemy::HasReacted()
{
	if (timeUntilReaction <= 0)
	{
		isReacting = false;
		return true;
	}
	else return false;
}

void CEnemy::SetAwareOfPlayer(bool a)
{
	awareOfPlayer = a;
}
CEnemy::~CEnemy()
{
}


void CEnemy::OnCollision(bool stopHor, bool stopVer)
{
	if (stopHor)
	{
		velocity.SetX(velocity[0] * -0.5);

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
	for (int i = 0; i < time; i++)
	{
		if (rand() % 2000 == 0)
		{
			xhairAngleRandomAddition = ((float)(rand() % 100 - 50) / 250.0f);
		}
	}
	if (isReacting)
		timeUntilReaction -= time;
	CAnimate::UpdateTime(time);
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
	else if (timeSincePlayerNotVisible < 1000)
	{
		return true;
	}
	else return false;
}