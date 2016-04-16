#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "PhysicsObject.h"

class CProjectile :
	public CPhysicsObject
{
public:
	CProjectile(int x, int y, int textureId, int size, bool firedByPlayer);
	CProjectile(int x, int y, int textureId, int width, int height, bool firedByPlayer, bool subjectToGravity);
	virtual ~CProjectile();
	virtual bool UpdateLife() = 0; 
	bool firedByPlayer;
	//explosionData
	enum explosionDataTypes
	{
		explosive,
		radius,
		impulse,
		damage,
		numberOfExplosionDataTypes
	};
	float explosionData[numberOfExplosionDataTypes];

	int animationData[numberOfDataTypes];
	int GetFrame();

};

#endif