#include "Player.h"
#include "Global.h"

CPlayer::CPlayer(int x, int y, int _textureId, int _size, vector<CWeapon*> & weaponVector) : CAnimate(x, y, _textureId, _size, weaponVector)
{
	weight = 1.5f;
	jumpStrength = 0.9f;
	moveSpeed = 0.002f;
	maxMoveSpeed = 0.26f;
	frameSizeX = 200;
	frameSizeY = 200;
	renderOrder = 2;

	animationData[idle][frameCountData] = 1;
	animationData[idle][msPerFrameData] = 1000;
	animationData[idle][startFrameData] = 0;
	animationData[move][frameCountData] = 1;
	animationData[move][msPerFrameData] = 1000;
	animationData[move][startFrameData] = 2;
	currentAnim = idle;

}
bool CPlayer::Die()
{

	CAnimate::Die();


	gameState = 4;
	return false;
}
int CPlayer::GetCurrentClip()
{
	return ammoInClip[weaponEquipped];
}
int CPlayer::GetCurrentClip(int slot)
{
	if (SlotOK(slot))
	{
		return ammoInClip[WeaponAtSlot(slot)];
	}
	else return 0;
}
void CPlayer::SetAmmoInClip(int slot, int amount)
{
	if (SlotOK(slot))
	{
		ammoInClip[WeaponAtSlot(slot)] = amount;
	}
}
int CPlayer::GetCurrentReserveAmmo()
{
	return ammoInInventory[weaponVector->at(weaponEquipped)->ammoType];
}
int CPlayer::GetCurrentReserveAmmo(int type)
{
	if (type >= 0 && type < numberOfAmmoTypes)
		return ammoInInventory[type];
	else return 0;
}
bool CPlayer::SlotOK(int slot)
{
	return (slot >= 0 && slot < numberOfSlots);
}
void CPlayer::AddWeaponToSlot(int weapon, int slot)
{
	if (weapon < numberOfWeapons && weapon >= 0 && SlotOK(slot))
	{
		inventorySlots[slot] = weapon;
	}
}

void CPlayer::SwapTwoSlots(int slot1, int slot2)
{
	if (SlotOK(slot1) && SlotOK(slot2))
	{
		int temp = inventorySlots[slot1];
		inventorySlots[slot1] = inventorySlots[slot2];
		inventorySlots[slot2] = temp;
		if (GetWeaponEquipped() == inventorySlots[slot2])
		{
			SwitchWeapon(slot2);
		}
		else if (GetWeaponEquipped() == inventorySlots[slot1])
		{
			SwitchWeapon(slot1);
		}
	}
}

int CPlayer::GetCurrentSlot()
{
	return currentSlot;
}

void CPlayer::EmptySlot(int slot)
{
	if (SlotOK(slot))
	{

		if (inventorySlots[slot] == GetWeaponEquipped())
		{
			SetWeaponEquipped(noWeapon);
		}
		inventorySlots[slot] = 0;
	}
}
int CPlayer::WeaponAtSlot(int index)
{
	if (SlotOK(index))
	{
		return inventorySlots[index];
	}
}

void CPlayer::SwitchWeapon(int slotIndex)
{
	if (SlotOK(slotIndex))
	{

		int weapon = inventorySlots[slotIndex];
		currentSlot = slotIndex;
		if (weapon != GetWeaponEquipped())
		{
			weaponVector->at(GetWeaponEquipped())->CeaseFire();
			SetFireDelay(weaponVector->at(GetWeaponEquipped())->fireDelay);
			CancelReload();
			SetWeaponEquipped(weapon);
		}
	}
}

void CPlayer::AddKey(int keyColor)
{
	if (keyColor >= 0 && keyColor <= numberOfColors)
		keyVector.push_back(keyColor);
}


void CPlayer::NextWeapon()
{
	for (int i = currentSlot; i < numberOfSlots; i++)
	{
		if (inventorySlots[i] != 0)
		{
			SwitchWeapon(i);
			return;
		}
	}
	for (int i = 0; i < currentSlot; i++)
	{
		if (inventorySlots[i] != 0)
		{
			SwitchWeapon(i);
			return;
		}
	}
}
void CPlayer::PreviousWeapon()
{
	for (int i = currentSlot; i >= 0; i--)
	{
		if (inventorySlots[i] != 0)
		{
			SwitchWeapon(i);
			return;
		}
	}
	for (int i = numberOfSlots - 1; i > currentSlot; i--)
	{
		if (inventorySlots[i] != 0)
		{
			SwitchWeapon(i);
			return;
		}
	}
}


CPlayer::~CPlayer()
{
}
void CPlayer::SetRotation(float _rotation)
{

	realRotation = _rotation;

	int cursorDistanceFromPlayer = sqrt(GetPointingDirectionX()*GetPointingDirectionX() + GetPointingDirectionY() * GetPointingDirectionY());

	if (cursorDistanceFromPlayer > GetWidth() / 2)
	{
		ChangeAnimation(move);
		if (_rotation > 90)
		{
			lookingLeft = true;
			_rotation -= 180;
		}
		else
		{
			lookingLeft = false;
		}

		if (_rotation < -40)
		{
			rotation = -40;
		}
		else if (_rotation > 40)
		{
			rotation = 40;
		}
		else rotation = _rotation;
	}
	else
	{
		rotation = 0;
		ChangeAnimation(idle);
	}


}
void CPlayer::OnCollision(bool stopHor, bool stopVer)
{
	if (stopHor)
	{
		velocity.SetX(0);
	}
	if (stopVer)
	{
		velocity.SetY(0);
	}
}


void CPlayer::RemoveKey(int key)
{
	for (int i = 0; i < keyVector.size(); i++)
	{
		if (keyVector[i] == key)
		{
			keyVector.erase(keyVector.begin() + i);
			return;
		}
	}
}
int CPlayer::HasKey(int key)
{
	int numberOfMatches = 0;
	for (int i = 0; i < keyVector.size(); i++)
	{
		if (keyVector[i] == key)
		{
			numberOfMatches++;
		}
	}
	return numberOfMatches;
}
int CPlayer::GetKeyAt(int keyIndex)
{
	if (keyIndex >= 0 && keyIndex < keyVector.size())
	{
		return keyVector[keyIndex];
	}
}

