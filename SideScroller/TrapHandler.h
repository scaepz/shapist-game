#ifndef TRAPHANDLER_H
#define	TRAPHANDLER_H

#include "SwimEffect.h"
#include "TrapObject.h"
#include <vector>
#include "CollisionDetector.h"
#include "VectorManager.h"
class CTrapHandler
{
public:
	CTrapHandler(CVectorManager * _vm);
	virtual ~CTrapHandler();
	
	void HandleTraps();
private:
	CVectorManager * vm;
	vector<CAnimate*>* animateVector;
	vector<CTrapObject*>* trapVector;
};

#endif