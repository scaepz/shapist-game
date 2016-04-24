#ifndef MELEEAI_H
#define MELEEAI_H
#include "AI.h"
class CMeleeAI :
	public CAI
{
public:
	virtual void Attack(CEnemy* self);
	CMeleeAI(CVectorManager *vm);
	virtual ~CMeleeAI();
};

#endif