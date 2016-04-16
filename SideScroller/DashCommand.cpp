#include "DashCommand.h"
#include <iostream>

#include <cmath>


CDashCommand::CDashCommand(CAnimate * obj, int dirX, int dirY) : CCommandObject(obj)
{
	
	//std::cout <<"player:" <<playerPosOnScreen[0] << ", " << playerPosOnScreen[1] << std::endl;
	//std::cout << "cursor:"<< dirX << ", " << dirY << std::endl;
	float a = sqrt(dirX*dirX + dirY*dirY) / (obj->maxMoveSpeed*10);
	velocityIncrease[0] = dirX / a;
	velocityIncrease[1] = dirY / a;

	exed = false;
	commandId = dash;
	duration = 115;
	staminaCost = 60;
}
bool CDashCommand::Execute()
{
	duration -= g_time;
	if (!exed)
	{
		if (obj->GetStamina() >= staminaCost)
		{
			exed = true;
			obj->LoseStamina(staminaCost);
			obj->velocity[0] = velocityIncrease[0];
			obj->velocity[1] = velocityIncrease[1];
			return false;
		}
		else
		{
			obj->FlashStamina();
			return true;
		}
		
	}
	else if (duration <= 0)
	{
		
		if (obj->velocity[1] > 0)
		{
			obj->velocity[1] /= 3;
		}
		else
		{
			obj->velocity[1] /= 10;
		}
		obj->velocity[0] /=10;
		return true;
	}
	else return false;
}
CDashCommand::~CDashCommand()
{
}
