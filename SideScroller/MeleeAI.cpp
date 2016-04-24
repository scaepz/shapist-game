#include "MeleeAI.h"


CMeleeAI::CMeleeAI(CVectorManager * vm) : CAI(vm)
{
}


CMeleeAI::~CMeleeAI()
{
}

void CMeleeAI::Attack(CEnemy* self)
{

	PlayerDirection verDir = GetPlayerVerticalDirection(self);

	self->currentAnim = self->move;
	int distance = DistanceToPlayer(self);
	PlayerDirection pDir = GetPlayerHorizontalDirection(self);

	if (verDir == PlayerDirection::above && GetHorizontalDistanceToPlayer(self) < 100)
	{
		if (ContinuousChance(1000))
			MoveRight(self);
		else if (ContinuousChance(1000))
			MoveLeft(self);
		else if (ContinuousChance(1000))
			Jump(self);
	}
	else if (pDir == PlayerDirection::left && distance > 20)
	{
			MoveLeft(self);
	}
	else if (pDir == PlayerDirection::right && distance > 20)
	{
			MoveRight(self);
	}
	else
	{
		SoftStop(self);
	}
	if (distance < 60)
	{
		if (self->GetPlayerVisible())
		{
			if (React(self))
				Fire(self);
		}
		else
		{
			CeaseFire(self);
		}
	}
	else if (ContinuousChance(3000))
	{
		Jump(self);
	}
}