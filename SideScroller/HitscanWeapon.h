#ifndef HITSCANWEAPON_H
#define HITSCANWEAPON_H
#include "Weapon.h"
#include "VectorManager.h"	
#include <vector>
#include "Tile.h"
#include "DevLaser.h"
#include "PhysicsObject.h"
#include "Global.h"
using std::vector;
class CHitscanWeapon :
	public CWeapon
{
public:

	CHitscanWeapon(CVectorManager *vm);
	virtual ~CHitscanWeapon();
private:

	void UpdateTime();
	int msSinceDmg;
	int msBetweenDmg;
	int range;
	int damage;
	double damageFalloff;
	vector<vector<CTile*>>* tileVector;
	vector<CDevLaser*> laserVector;
	vector<CBaseObject*>* baseVector;
	vector<CBaseObject*>* solidVector;
	CVectorManager * vm;
	int standardTileSize;
	bool Attack(int x, int y, float dirX, float dirY);
	void CeaseFire();
};

#endif