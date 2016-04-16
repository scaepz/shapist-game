#pragma once
#include "AI.h"

class CSimpleTestAI :
	public CAI
{
public:
	CSimpleTestAI(CVectorManager *vm);
	void MakeDecision(CEnemy* self);
	virtual ~CSimpleTestAI();
	


};

