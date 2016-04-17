#pragma once
#include "AI.h"

class CSimpleTestAI :
	private CAI
{
public:
	CSimpleTestAI(CVectorManager *vm);
	void MakeDecision(CEnemy* self);
	virtual ~CSimpleTestAI();
	


};

