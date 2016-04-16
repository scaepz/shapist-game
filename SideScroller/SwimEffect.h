#ifndef SWIMEFFECT_H
#define SWIMEFFECT_H
#include "Effect.h"
class CSwimEffect :
	public CEffect
{
public:
	CSwimEffect(CAnimate * obj, float viscosity);
	virtual ~CSwimEffect();
	bool Execute();




private:
	float viscosity;
	float saveMaxSpeed;
	bool exedOnce = false;
};

#endif