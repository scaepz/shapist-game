#ifndef BULLETWEAPON_H
#define BULLETWEAPON_H
#include "Weapon.h"
#include "VectorManager.h"
#include "ParticleEngine.h"
class CBulletWeapon :
	public CWeapon
{
public:
	CBulletWeapon(CVectorManager * vm);
	bool Attack(int x, int y, float angle);
	virtual ~CBulletWeapon();
	CVectorManager * vm;
	int maxLength;
	int numberOfPellets;
	float fireConeTightness; // lower value more spread
	int damage;
	float tracerIntensity;
	bool semiAutomatic;

	void Update();
	float recoilIncreasePerBullet;
	float spreadIncreasePerBullet;

	float maxExtraRecoil;
	float maxExtraSpread;

private:
	float currentExtraRecoil;
	float currentExtraSpread;
	int totalDamage; //used to determine hit sound
};
#endif 


