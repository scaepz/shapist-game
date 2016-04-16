#ifndef TRAPOBJECT_H
#define TRAPOBJECT_H
#include "BaseObject.h"
#include "Animate.h"
#include "Global.h"
class CTrapObject :
	public CBaseObject
{
public:
	CTrapObject(int x, int y);

	int damage;
	int frequencyInMs;
	virtual void AffectVictim(CAnimate* victim) = 0;
	enum animations
	{
		idle,
		death,
		numberOfAnimations
	};
	int animationData[numberOfAnimations][numberOfDataTypes];
	void UpdateTime(int time);
	virtual ~CTrapObject();
	int timeSinceDmgDealt;

	
};

#endif