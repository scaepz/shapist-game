#ifndef ANIMATE_H
#define ANIMATE_H
#include "WeaponsEnum.h"
#include "PhysicsObject.h"
#include "BaseObject.h"
#include "Weapon.h"
#include "KickBack.h"
#include "ItemEnum.h"
#include <vector>
using std::vector;



class CAnimate :
	public CPhysicsObject
{
public:
	CAnimate(int x, int y, int textureId, int size, vector<CWeapon*> & weaponVector);
	virtual ~CAnimate();
	float jumpStrength;
	bool Move(bool dir, int ms);
	bool AirMove(bool dir, int ms);
	float moveSpeed;
	float maxMoveSpeed;
	void TakeDamage(int damage);

	virtual int GetWidth();
	virtual int GetHeight();
	virtual int GetX();
	virtual int GetY();
	int GetHP();

	void Init();
	enum animations
	{
		idle,
		idle_rare,
		move,
		attack,
		dmg,
		death,
		jump,
		numberOfAnimations
	};
	CKickBack *kickback = nullptr;
	int animationData[numberOfAnimations][numberOfDataTypes];
	void ChangeAnimation(int animation);
	int GetFrame();
	
	int GetWeaponFrame();
	int weaponAnimation;
	int weaponAnimationMs;

	void ApplyViscocityFriction(float viscocity);

	int groupNumber;
	virtual void UpdateTime(int time);

	//Weaponstuff
	vector<CWeapon*>* weaponVector;

	bool CancelAttack();
	
	void SetSwitchDelay(int ms);
	void SetFireDelay(int ms);
	void SetReloadDelay(int ms);

	virtual bool Attack();

	int GetWeaponEquipped();
	bool SetWeaponEquipped(int weapon);
//	virtual bool HasWeapon(int weapon);
	//virtual void AddWeapon(int weapon);
	//	virtual void RemoveWeapon(int weapon);

	bool IsHealthy();
	void SetHP(int hp);
	void AddHP(int hp);

	virtual bool Die();
	bool IsDying();

	bool IsReloading();
	void Reload();
	void CancelReload();
	float GetXhairAngle();

	//xhair angle new, pointing dir old but currently used in setrotation. cleaning this up is low priority.
	void SetXhairAngle(float angle);
	void SetPointingDirection(float dirX, float dirY);
	float GetPointingDirectionX();
	float GetPointingDirectionY();
	virtual float GetXhairAngleDeg();



	void SetLoot(int loot);
	int GetLoot();
	void AddAmmo(int type, int amount);

	//make stamina hud bar flash red
	void FlashStamina();
	int GetStaminaRedness();
	int GetStamina();
	bool LoseStamina(int amount); //returns false if animate has too little stamina
	void SetStaminaRegenPerMs(int staminaPerMs);
	void StartWeaponAnimation(int animation);
	int ammoInClip[numberOfWeapons];
protected:
	bool weaponInventory[numberOfWeapons];
	int ammoInInventory[numberOfAmmoTypes];
	int weaponEquipped = 0;
private:
	bool isAttacking = false;
	void SetDamageDelay(int ms);
	int damageDelay;
	const int MAX_STAMINA = 100;
	float staminaRegenPerMs;
	float stamina;
	int loot;
	
	float xhairAngle;
	float pointingDirection[2];
	int hp;
	int fireDelay;

	bool reloading;

	float staminaRedness;
	bool dying;
	int deathTime;


};

#endif