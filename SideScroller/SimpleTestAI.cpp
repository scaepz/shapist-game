#include "SimpleTestAI.h"
#include "SDL.h"
#include <iostream>
#include "VectorManager.h"
CSimpleTestAI::CSimpleTestAI(CVectorManager *_vm) : CAI(_vm)
{
}


CSimpleTestAI::~CSimpleTestAI()
{
}


void CSimpleTestAI::MakeDecision(CEnemy* self)
{
	if (self->GetPlayerVisibleStale())
	{
		self->SetPlayerVisible(IsPlayerInLineOfSight(1000, self));
	}
	if (self->GetPlayerVisible())
	{
		int xDistance = player->pos[0] + (player->GetWidth() / 2) - self->pos[0] + (self->GetWidth() / 2);
		if (xDistance > 130
			|| xDistance < -30)
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
		else
		{


			CMoveCommand moveLeft(self, 0);
			vm->DeleteObject(&moveLeft);

			CMoveCommand moveRight(self, 1);
			vm->DeleteObject(&moveRight);


		}
	}
	srand(SDL_GetTicks());
	if (rand() % 500 + 1 == 29)
	{
		CJumpCommand * jump = new CJumpCommand(self);
		std::cout << "JUMP!";
		vm->AddObject(jump);
	}
	else if (rand() % 500 + 1 == 30)
	{
		CAttackCommand * attack = new CAttackCommand(self);
		vm->AddObject(attack);
	}

}
