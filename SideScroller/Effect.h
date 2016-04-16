#ifndef EFFECT_H
#define EFFECT_H
#include "Animate.h"
#include "Global.h"
class CEffect
{
public:
	CEffect(CAnimate * _obj);
	virtual ~CEffect();

	int effectId;
	enum ids
	{
		swim,
		numberOfEffects
	};



	int durationInMs;
	int timeSinceExecute = 0;
	virtual bool Execute() = 0;  // Returns true if effect has run out
	CAnimate * obj; // Target

	bool AddTime(int ms); // returns true if time <= 0, then resets time
	

};

#endif