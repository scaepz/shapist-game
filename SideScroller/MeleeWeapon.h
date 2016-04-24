#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H
#include "Weapon.h"
#include "VectorManager.h"
class CMeleeWeapon :
	public CWeapon
{
public:

	CMeleeWeapon(CVectorManager * vm);
	virtual ~CMeleeWeapon();
	bool Attack(int x, int y, float angle, CBaseObject* attacker);
	int GetFrame(int ms, int animation);
	int maxLength;
	int staminaCost;
	int damage;
	int damageDelay;
	void Update();
	int timeSinceAnimationStart;

private:

	
	int currentAnimation;
	void StartAnimation(int);

	CVectorManager * vm;

};

#endif