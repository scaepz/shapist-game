#ifndef AGGRESSIVEAI_H
#define AGGRESSIVEAI_H

#include "AI.h"
#include "Enemy.h"

class CAggressiveAI : // Moves toward player, jumps at random times, perfect for melee, kamikazee
	public CAI
{
public:
	CAggressiveAI(CVectorManager *_vm);
	void MakeDecision(CEnemy* self);
	virtual ~CAggressiveAI();
};

#endif