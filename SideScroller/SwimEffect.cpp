#include "SwimEffect.h"


CSwimEffect::CSwimEffect(CAnimate * obj, float _viscosity) : CEffect(obj)
{
	viscosity = _viscosity;
	durationInMs = 40;
	effectId = swim;
}


CSwimEffect::~CSwimEffect()
{
}

bool CSwimEffect::Execute()
{
	if (!exedOnce)
	{
		saveMaxSpeed = obj->maxMoveSpeed - obj->maxMoveSpeed * 0.9;
		obj->maxMoveSpeed *= 0.9;
		exedOnce = true;
	}
	obj->underWater = true;
	
	for (int i = 0; i < g_time; i++)
	{
		obj->ApplyViscocityFriction(viscosity);
	}
	if (AddTime(g_time))
	{
		//revert changes
		obj->maxMoveSpeed += saveMaxSpeed;
		obj->underWater = false;
		return true;
	}
	else return false;
}