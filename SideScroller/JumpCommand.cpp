#include "JumpCommand.h"
#include <iostream>

CJumpCommand::CJumpCommand(CAnimate * _obj) : CCommandObject(_obj)
{
	commandId = jump;
	tries = 100;
	staminaCost = 25;
}


CJumpCommand::~CJumpCommand()
{
}

bool CJumpCommand::Execute()
{
	if (tries > 0)
	{
		if (obj->underWater) //if underwater is true jumpcommand is swimup
		{
			if (obj->velocity[1] > obj->maxMoveSpeed *-1)
			{
				//swim
				if (obj->velocity[1] - (obj->moveSpeed * g_time*1.5) < obj->maxMoveSpeed*-1)
				{

					obj->velocity[1] = obj->maxMoveSpeed*-1;
					return false; //return false even if we've reached maxmovespeed, because otherwise new commandobjects are just going to keep being created until player lets go of jump
				}
				else
				{
					CVector2f swimUp(0, (float)(-1 * obj->moveSpeed*g_time*1.5));
					obj->AddVelocity(swimUp);

					return false; //return false if we've yet to reach maxmovespeed
				}
			}
			else
			{

				return true;
			}//return true if we're already moving faster than maxmovespeed
		}
		else if (obj->OnGround() && obj->velocity[1] > -0.5*obj->jumpStrength) //if OnGround is true then object is about one pixel above something solid. The second part of the if-statement is a bandaid fix which makes it impossible to amass upward velocity by running towards an object and spamming jump
		{
			//jump
			if (obj->LoseStamina(staminaCost))
			{
				CVector2f jump(0, -1 * obj->jumpStrength);
				obj->AddVelocity(jump);
				obj->SetOnGround(false);
			}
			return true; //return true when jump succeeds or stamina fail (object can be deleted)

		}
		else
		{

			// in air
			tries -= g_time; // this gives the player a short grace period. If the player is still in air when the command is executed, the command will keep trying to execute for as many ms as the value of the integer "tries"
			return false;
		}
	}
	else return true; // if tries is < 0 the grace period is over and the object can be deleted. 
}