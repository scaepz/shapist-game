#ifndef FLUID_H
#define FLUID_H
#include "TrapObject.h"
#include "SwimEffect.h"
#include "VectorManager.h"
class CFluid :
	public CTrapObject
{
public:
	CFluid(int x, int y, CVectorManager * _vm, float viscocity);
	virtual ~CFluid();

	void AffectVictim(CAnimate* victim);
	float viscocity;
	int GetFrame();
private:
	CVectorManager *vm;
};

#endif