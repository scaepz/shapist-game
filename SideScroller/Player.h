#ifndef PLAYER_H
#define PLAYER_H
#include "Animate.h"
#include "PhysicsObject.h"
#include "WeaponsEnum.h"
const int numberOfSlots = 4;
class CPlayer : public CAnimate
{
public:
	CPlayer(int x, int y, int _textureId, int _size, vector<CWeapon*> & weaponVector);
	virtual ~CPlayer();
	void OnCollision(bool stopHor, bool stopVer);
	
	void SetRotation(float _rotation);
	
	
	void SwitchWeapon(int weapon);
	void NextWeapon();
	void PreviousWeapon();
	void AddWeaponToSlot(int weapon, int slot);
	void EmptySlot(int slot);
	void SwapTwoSlots(int slot1, int slot2);
	int WeaponAtSlot(int index);
	int GetCurrentSlot();
	bool Die();
	int GetCurrentClip();
	int GetCurrentClip(int slot);
	int GetCurrentReserveAmmo();
	int GetCurrentReserveAmmo(int weapon);
	void SetAmmoInClip(int slot, int amount);

	void AddKey(int);
	void RemoveKey(int);
	int HasKey(int); //returns number of matching keys
	int GetKeyAt(int);
	enum keyColors
	{
		yellow,
		red,
		purple,
		numberOfColors
	};

private:
	vector<int> keyVector;
	bool SlotOK(int slot);
	int inventorySlots[numberOfSlots];
	int currentSlot;
};

#endif