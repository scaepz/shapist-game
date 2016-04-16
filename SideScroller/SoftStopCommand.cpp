#include "SoftStopCommand.h"


CSoftStopCommand::CSoftStopCommand(CAnimate* obj) : CCommandObject(obj)
{
	commandId = softstop;
}

bool CSoftStopCommand::Execute()
{

	if (obj->OnGround())
	{
		return true;
	}
	else
	{
		if (obj->velocity[0] < 0)
		{
			if (obj->velocity[0] + obj->moveSpeed*g_time*0.15 >= 0)
			{
				obj->velocity[0] = 0;
				return true;
			}
			else
			{
				obj->velocity[0] += obj->moveSpeed*g_time*0.15;
				return false;
			}
		}
		else if (obj->velocity[0] > 0)
		{
			if (obj->velocity[0] - obj->moveSpeed*g_time*0.15 <= 0)
			{
				obj->velocity[0] = 0;
				return true;
			}
			else
			{
				obj->velocity[0] -= obj->moveSpeed*g_time*0.15;
				return false;
			}
		}
		else return true;
	}

}

CSoftStopCommand::~CSoftStopCommand()
{
}
