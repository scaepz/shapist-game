#include "Effect.h"


CEffect::CEffect(CAnimate * _obj)
{
	obj = _obj;
}


CEffect::~CEffect()
{
}

bool CEffect::AddTime(int ms)
{
	timeSinceExecute += ms;
	if (timeSinceExecute >= durationInMs)
	{
		timeSinceExecute = 0;
		return true;
	}
	else return false;
}
