#include "MoveCommand.h"


CMoveCommand::CMoveCommand(CAnimate * _obj, int _dir) : CCommandObject(_obj)
{
	airMovedOnce = false;
	dir = _dir;
	if (_dir == 0)
	{
		commandId = moveleft;
	}
	else commandId = moveright;
}


CMoveCommand::~CMoveCommand()
{
}

bool CMoveCommand::Execute()
{

	if (obj->OnGround() || obj->underWater)
	{
		return (obj->Move(dir, g_time));
	}
	else
	{
		if (!airMovedOnce)
		{
			airMovedOnce = obj->AirMove(dir, g_time);
		}
		return false;
	}

}