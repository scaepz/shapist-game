#ifndef RANGEDAI_H
#define RANGEDAI_H

//likes to keep itself inside a specific distance range from player
#include "AI.h"
#include "Enemy.h"
class CRangedAI :
	public CAI
{
public:
	CRangedAI(CVectorManager * vm);
	void MakeDecision(CEnemy * self);
	virtual ~CRangedAI();
};

#endif