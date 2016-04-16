#ifndef CAUTIOUSRANGEDAI_H
#define CAUTIOUSRANGEDAI_H
#include "AI.h"
class CCautiousRangedAI :
	public CAI
{
public:
	CCautiousRangedAI(CVectorManager * vm);
	void MakeDecision(CEnemy * self);
	virtual ~CCautiousRangedAI();
};

#endif