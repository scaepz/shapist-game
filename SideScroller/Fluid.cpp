#include "Fluid.h"


CFluid::CFluid(int x, int y, CVectorManager * _vm, float _viscocity) : CTrapObject(x, y)
{
	renderOrder = 4;
	vm = _vm;
	damage = 1;
	frequencyInMs = 150;
	ChangeAnimation(idle);
	viscocity = _viscocity;
}


CFluid::~CFluid()
{
}
int CFluid::GetFrame()
{
	if (timeSinceAnimationStart / animationData[currentAnim][msPerFrameData] < animationData[currentAnim][frameCountData])
	{
		return animationData[currentAnim][startFrameData] + timeSinceAnimationStart / animationData[currentAnim][msPerFrameData];
	}
	else
	{
		ResetTime();
		return GetFrame();
	}
}
void CFluid::AffectVictim(CAnimate* victim)
{
	CEffect * FluidSwim = new CSwimEffect(victim, viscocity);
	vm->AddObject(FluidSwim);
	if (timeSinceDmgDealt > frequencyInMs)
	{
		timeSinceDmgDealt = 0;
		victim->TakeDamage(damage);
	}
}