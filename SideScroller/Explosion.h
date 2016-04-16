#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "BaseObject.h"
class CExplosion : public CBaseObject
{
public:
	CExplosion(int size, float impulse, int damage, int x, int y, int _type);
	virtual ~CExplosion();
	

	float impulse;
	int radius;
	int damage;
	int GetFrame();
	bool alreadyImpulsed;
	int type;
	int impulseCentre[2];
	enum animations
	{
		blow,
		numberOfAnimations
	};
	int animationData[numberOfAnimations][numberOfDataTypes];

	bool IsHealthy()
	{
		if (timeSinceAnimationStart < animationData[blow][frameCountData] * animationData[blow][msPerFrameData])
		{
			return true;
		}
		else return false;
	}
	
};

#endif