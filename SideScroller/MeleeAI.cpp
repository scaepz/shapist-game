#include "MeleeAI.h"


CMeleeAI::CMeleeAI(CVectorManager * vm) : CAI(vm)
{
}


CMeleeAI::~CMeleeAI()
{
}

void CMeleeAI::Attack(CEnemy* self)
{

	if (self->currentActivity != CEnemy::CurrentActivity::fighting)
	{
		self->currentActivity = CEnemy::CurrentActivity::fighting;
	}
	self->currentAnim = self->move;
	int distance = DistanceToPlayer(self);
	PlayerDirection pDir = GetPlayerHorizontalDirection(self);

	if (pDir == PlayerDirection::left && distance > 20)
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