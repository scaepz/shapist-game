#include "RetreatingAI.h"


CRetreatingAI::CRetreatingAI(CVectorManager * vm) : CAI(vm)
{
}


CRetreatingAI::~CRetreatingAI()
{
}


void CRetreatingAI::MakeDecision(CEnemy * self)
{
	if (self->GetPlayerVisibleStale())
	{
		self->SetPlayerVisible(IsPlayerInLineOfSight(self->aiParam[0], self));
	}
	if (self->IsAwareOfPlayer())
	{
		if (player->pos[0] > self->pos[0])
		{
			if (self->GetTimeUntilReaction() <= 0)
			{
				self->ResetTimeUntilReaction();

				CMoveCommand moveRight(self, 1);
				vm->DeleteObject(&moveRight);

				CCommandObject * moveLeft = new CMoveCommand(self, 0);
				vm->AddObject(moveLeft);

			}
			self->UpdateTimeUntilReaction(g_time);
		}
		else
		{
			if (self->GetTimeUntilReaction() <= 0)
			{
				self->ResetTimeUntilReaction();

				//remove left commands
				CMoveCommand moveLeft(self, 0);
				vm->DeleteObject(&moveLeft);

				//add right command
				CCommandObject * moveRight = new CMoveCommand(self, 1);
				vm->AddObject(moveRight);
			}
			self->UpdateTimeUntilReaction(g_time);
		}

	}
	srand(SDL_GetTicks() + GetIncrementalInt());
	if (rand() % 300 + 1 == 29)
	{
		CJumpCommand * jump = new CJumpCommand(self);
		vm->AddObject(jump);
	}
}