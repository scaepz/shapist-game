#include "RangedAI.h"


CRangedAI::CRangedAI(CVectorManager * vm) : CAI(vm)
{
}


CRangedAI::~CRangedAI()
{
}
void CRangedAI::MakeDecision(CEnemy * self)
{
	if (self->GetPlayerVisibleStale())
	{
		self->SetPlayerVisible(IsPlayerInLineOfSight(self->aiParam[0], self));
	}
	if (self->GetPlayerVisible())
	{
		int xDistance = (player->pos[0] + (player->GetWidth() / 2)) - (self->pos[0] + (self->GetWidth() / 2));
		self->SetPointingDirection(player->GetX() - self->GetX(), player->GetY() - self->GetY());
		if (abs(xDistance) > self->aiParam[1])
		{
			if (player->pos[0] > self->pos[0])
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
			else
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
		}
		else if (abs(xDistance) < self->aiParam[2])
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
		else
		{
			CMoveCommand moveLeft(self, 0);
			vm->DeleteObject(&moveLeft);

			CMoveCommand moveRight(self, 1);
			vm->DeleteObject(&moveRight);

			CAttackCommand * attack = new CAttackCommand(self);
			vm->AddObject(attack);

		}
	}
	srand(SDL_GetTicks() + GetIncrementalInt());
	if (rand() % 500 + 1 == 29)
	{
		CJumpCommand * jump = new CJumpCommand(self);
		vm->AddObject(jump);
	}
}