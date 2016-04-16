#include "StopCommand.h"


CStopCommand::CStopCommand(CAnimate * _obj) : CCommandObject(_obj)
{
	commandId = stop;
	staminaCost = 10;
}


CStopCommand::~CStopCommand()
{
}

bool CStopCommand::Execute()
{

	if (obj->underWater)
	{
		if (obj->velocity[1] < obj->maxMoveSpeed)
		{
			//swim down
			if (obj->velocity[1] + obj->moveSpeed*g_time < obj->maxMoveSpeed)
			{
				CVector2f swimDown(0, (float)(obj->moveSpeed*g_time));
				obj->AddVelocity(swimDown);
				return false;
			}
			else
			{
				obj->velocity[1] = obj->maxMoveSpeed;
				return true;
			}
		}
		else return true;
	}
	else
	{
		//stop
		if (obj->LoseStamina(10))
		{
			obj->velocity[0] = 0;
		}
		return true;
	}



}