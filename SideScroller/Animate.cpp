#include "Animate.h"
#include "Global.h"
#include "SDL.h"
#include "vector2.h"
CAnimate::CAnimate(int x, int y, int textureId, int size, vector<CWeapon*> & _weaponVector) : CPhysicsObject(x, y, textureId, size)
{
	kickback = new CKickBack();
	smallObject = false;
	fireDelay = 0;

	weaponVector = &_weaponVector;
	renderOrder = 2;
	SetLoot(redHP);
	stamina = MAX_STAMINA;
	staminaRegenPerMs = 0.035f;
	staminaRedness = 0;
}

CAnimate::~CAnimate()
{
	if (kickback != nullptr)
		delete kickback;
}

void CAnimate::AddAmmo(int ammoType, int amount)
{
	if (ammoType >= 0 && ammoType < numberOfAmmoTypes)
	{
		ammoInInventory[ammoType] += amount;
	}
}


void CAnimate::Init()
{
	//time it takes to die is number of death frames * ms per frame
	deathTime = animationData[death][0] * animationData[death][1];
}

int CAnimate::GetFrame()
{
	if (animationData[currentAnim][frameCountData] != 0)
	{
		if (timeSinceAnimationStart / animationData[currentAnim][1] < animationData[currentAnim][frameCountData])
		{
			return animationData[currentAnim][startFrameData] + timeSinceAnimationStart / animationData[currentAnim][msPerFrameData];
		}
		else
		{
			if (currentAnim == idle_rare) ChangeAnimation(idle);
			ResetTime();
			return GetFrame();
		}
	}
	else ChangeAnimation(idle);

}
void CAnimate::SetLoot(int _loot)
{
	if (_loot > 0 && _loot < numberOfItems)
	{
		loot = _loot;
	}
	else loot = 0;
}
int CAnimate::GetLoot()
{
	return loot;
}
void CAnimate::ApplyViscocityFriction(float viscocity)
{
	velocity[0] *= viscocity;
	velocity[1] *= viscocity;
}

int CAnimate::GetWidth()
{
	return length;
}

bool CAnimate::IsHealthy()
{
	if (hp > 0)
	{
		return true;
	}
	else return false;
}

void CAnimate::AddHP(int _hp)
{
	hp += _hp;
}

int CAnimate::GetHeight()
{
	return height;
}
int CAnimate::GetX()
{
	return pos[0];
}
int CAnimate::GetY()
{
	return pos[1];
}
void CAnimate::ChangeAnimation(int animation)
{
	if (animation == idle) lookingLeft = false;
	if (animationData[animation][frameCountData] == 0) return;
	if (currentAnim != animation && !IsDying())
	{
		currentAnim = animation;
		ResetTime();
	}
}
bool CAnimate::Move(bool dir, int ms)
{
	if (dir == 0) // left
	{

		if (velocity[0] > maxMoveSpeed*-1)
		{
			if (velocity[0] + ((moveSpeed*-1) * ms) < maxMoveSpeed*-1)
			{
				velocity[0] = maxMoveSpeed * -1;
				return false;
			}
			else
			{
				CVector2f moveleft((float)(moveSpeed *ms*-1), 0);
				AddVelocity(moveleft);
			}
			return false;

		}
		else return true;
	}
	else //right
	{

		if (velocity[0] < maxMoveSpeed)
		{
			if (velocity[0] + (moveSpeed * ms) > maxMoveSpeed)
			{
				velocity[0] = maxMoveSpeed;
				return false;
			}
			else
			{
				CVector2f moveleft((float)(moveSpeed *ms), 0);
				AddVelocity(moveleft);
			}
			return false;

		}
		else return true;
	}
}



void CAnimate::TakeDamage(int damage)
{
	hp -= damage;
}

bool CAnimate::AirMove(bool dir, int ms)
{
	if (dir == 0)
	{
		//left


		if (velocity[0] > 0)
		{
			if (velocity[0] + maxMoveSpeed*-0.005*ms > 0)
			{
				//small increment to turn in air
				velocity[0] += maxMoveSpeed*-0.005*ms;
				return false;
			}
			else
			{
				//max selfmovement in air
				velocity[0] = maxMoveSpeed*-0.3;
				return true;
			}
		}
		else if (velocity[0] > maxMoveSpeed*-0.3)
		{
			//max selfmovement in air
			velocity[0] = maxMoveSpeed*-0.3;
			return true;
		}
		else return true;

	}
	else // right
	{

		if (velocity[0] < 0)
		{
			if (velocity[0] + maxMoveSpeed*-0.005*ms < 0)
			{
				//small increment to turn in air
				velocity[0] += maxMoveSpeed*0.005*ms;
				return false;
			}
			else
			{
				//max selfmovement in air
				velocity[0] = maxMoveSpeed*0.3;
				return true;
			}
		}
		else if (velocity[0] < maxMoveSpeed*0.3)
		{
			//max selfmovement in air
			velocity[0] = maxMoveSpeed*0.3;
			return true;
		}
		else return true;

	}
}



void CAnimate::SetFireDelay(int ms)
{
	fireDelay = ms;
}

int CAnimate::GetHP()
{
	return hp;
}

bool CAnimate::IsReloading()
{
	return reloading;
}

int CAnimate::GetWeaponEquipped()
{
	return weaponEquipped;
}

void CAnimate::SetXhairAngle(float angle)
{
	if (angle > 3.1415f)
	{
		xhairAngle = angle - (2.0f * 3.1415f);
	}
	else if (angle < -3.1415f)
	{
		xhairAngle = angle + (2.0f * 3.1415f);
	}
	else
	{
		xhairAngle = angle;
	}
	if (xhairAngle < -1.57f || xhairAngle > 1.57f)
		lookingLeft = true;
	else lookingLeft = false;
}

float CAnimate::GetXhairAngle()
{
	return xhairAngle;
}

void CAnimate::SetPointingDirection(float dirX, float dirY)
{
	pointingDirection[0] = dirX;
	pointingDirection[1] = dirY;
	xhairAngle = atan2(dirY, dirX);
}
float CAnimate::GetPointingDirectionX()
{
	return pointingDirection[0];
}
float CAnimate::GetPointingDirectionY()
{
	return pointingDirection[1];
}
float CAnimate::GetXhairAngleDeg()
{
	return xhairAngle * 180 / 3.14f;
}
void CAnimate::SetStaminaRegenPerMs(int staminaPerMs)
{
	staminaRegenPerMs = staminaPerMs;
}

int CAnimate::GetStamina()
{
	return stamina;
}
bool CAnimate::LoseStamina(int amount)
{
	if (amount <= stamina)
	{
		stamina -= amount;
		if (stamina > MAX_STAMINA) stamina = MAX_STAMINA;
		return true;
	}
	else
	{
		FlashStamina(); return false;
	}
}

void CAnimate::FlashStamina()//make stamina hud bar flash red
{
	staminaRedness = 250;
}
int CAnimate::GetStaminaRedness()
{
	return staminaRedness;
}
void CAnimate::UpdateTime(int time)
{
	AddTime(time);

	if (stamina != MAX_STAMINA)
	{
		stamina += staminaRegenPerMs * g_time;
		if (stamina > MAX_STAMINA) stamina = MAX_STAMINA;
	}
	if (staminaRedness > 0)
	{
		staminaRedness -= (float)g_time / 1.5f;
	}
	if (staminaRedness < 0) staminaRedness = 0;

	kickback->Update(time);
	if (fireDelay > 0)
	{
		fireDelay -= time;
	}
	if (reloading && fireDelay <= 0)
	{
		if (ammoInInventory[weaponVector->at(GetWeaponEquipped())->ammoType] < weaponVector->at(GetWeaponEquipped())->clipSize)
		{
			ammoInClip[GetWeaponEquipped()] = ammoInInventory[weaponVector->at(GetWeaponEquipped())->ammoType];
			ammoInInventory[GetWeaponEquipped()] = 0;
		}
		else
		{
			ammoInClip[GetWeaponEquipped()] = weaponVector->at(GetWeaponEquipped())->clipSize;
			ammoInInventory[weaponVector->at(GetWeaponEquipped())->ammoType] -= weaponVector->at(GetWeaponEquipped())->clipSize;
		}
		reloading = false;
	}
	if (IsDying())
	{
		deathTime -= g_time;
	}
	weaponAnimationMs += g_time;
}
void CAnimate::StartWeaponAnimation(int animation)
{
	if (animation == 0) isAttacking = false;
	if (weaponVector->at(weaponEquipped)->GetFrame(0, animation) == -1)
	{
		if (animation == 2) StartWeaponAnimation(1);
		else
		{
			StartWeaponAnimation(0);
		}
	}
	else
	{
		weaponAnimation = animation;
		weaponAnimationMs = 0;
	}
}
int CAnimate::GetWeaponFrame()
{
	int frame = weaponVector->at(weaponEquipped)->GetFrame(weaponAnimationMs, weaponAnimation);
	if (frame == -1)
	{
		//(re)start idle animation;
		weaponAnimation = 0;
		weaponAnimationMs = 0;
		return weaponVector->at(weaponEquipped)->GetFrame(weaponAnimationMs, weaponAnimation);
	}
	return frame;
}

bool CAnimate::CancelAttack()
{
	if (weaponAnimationMs < weaponVector->at(weaponEquipped)->animPointOfNoReturnInMs)
	{
		StartWeaponAnimation(0);
		isAttacking = false;
		return true;
	}
	return false;
}

bool CAnimate::Attack()
{
	if (ammoInClip[GetWeaponEquipped()] > 0 || weaponVector->at(GetWeaponEquipped())->ammoCost == 0)
	{
		if (fireDelay <= 0 && !IsReloading() && currentAnim != idle)
		{
			if (ammoInClip[GetWeaponEquipped()] == 0 && weaponVector->at(GetWeaponEquipped())->ammoCost != 0)
			{
				Reload();
			}
			if (!isAttacking)
			{
				isAttacking = true;
				StartWeaponAnimation(rand() % 2 + 1); //Random 1 or 2 because some weapons have two attack animations. The method takes care of the mess if a weapon only has one (or none).
			}
			if (weaponAnimationMs > weaponVector->at(weaponEquipped)->animPointOfNoReturnInMs) //at pointOfNoReturn damage can be dealt
			{
				if (LoseStamina(weaponVector->at(GetWeaponEquipped())->staminaCost));
				{
					SetFireDelay(weaponVector->at(GetWeaponEquipped())->fireDelay);
					ammoInClip[GetWeaponEquipped()] -= weaponVector->at(GetWeaponEquipped())->ammoCost;
					kickback->NewKickback();
					return (weaponVector->at(weaponEquipped)->Attack(pos[0] + (GetWidth() / 2), pos[1] + (GetHeight() / 2), GetXhairAngle()));
				}
			}
			else return false;

		}
		else return false;
	}
	else
	{
		if (!reloading)
		{
			//reload
			Reload();
			return true;
		}
		else return false;
	}
}

void CAnimate::SetHP(int _hp)
{
	hp = _hp;
}

bool CAnimate::Die()
{
	if (!IsDying())
	{
		ChangeAnimation(death);
		dying = true;
	}

	if (deathTime > 0)
	{
		return false;
	}
	else return true;
}

bool CAnimate::IsDying()
{
	return dying;
}

bool CAnimate::SetWeaponEquipped(int weapon)
{

	kickback->SetKickBack(weaponVector->at(weapon)->kickBackX,
		weaponVector->at(weapon)->kickBackY,
		weaponVector->at(weapon)->kickBackRot,
		weaponVector->at(weapon)->kickBackTime);
	weaponEquipped = weapon;
	return true;

}
/*
bool CAnimate::HasWeapon(int weapon)
{
if (weapon < numberOfWeapons && weapon >= 0)
return weaponInventory[weapon];
else return false;
}


void CAnimate::AddWeapon(int weapon)
{
if (weapon < numberOfWeapons && weapon >= 0)
{
weaponInventory[weapon] = true;
}
}
void CAnimate::RemoveWeapon(int weapon)
{
if (weapon < numberOfWeapons && weapon >= 0)
{
weaponInventory[weapon] = false;
}
}
*/
void CAnimate::Reload()
{
	if (!IsReloading())
	{
		SetFireDelay(weaponVector->at(GetWeaponEquipped())->reloadDelay);
		reloading = true;
	}
}

void CAnimate::CancelReload()
{
	if (IsReloading())
	{
		SetFireDelay(0);
		reloading = false;
	}
}


