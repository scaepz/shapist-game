#ifndef RETREATINGAI_H
#define RETREATINGAI_H
#include "AI.h"
#include "Enemy.h"
class CRetreatingAI :
	public CAI
{
public:
	CRetreatingAI(CVectorManager * vm);
	void MakeDecision(CEnemy * self);
	~CRetreatingAI();
};

#endif