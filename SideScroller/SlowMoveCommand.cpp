#include "SlowMoveCommand.h"


CSlowMoveCommand::CSlowMoveCommand(CAnimate * _obj, int _dir) : CCommandObject(_obj)
{
	airMovedOnce = false;
	dir = _dir;
	if (_dir == 0)
	{
		commandId = moveleft;
	}
	else commandId = moveright;
}


CSlowMoveCommand::~CSlowMoveCommand()
{
}

bool CSlowMoveCommand::Execute()
{

	if (obj->OnGround() || obj->underWater)
	{
		return (obj->Move(dir, g_time/3));
	}
	else
	{
		if (!airMovedOnce)
		{
			airMovedOnce = obj->AirMove(dir, g_time/3);
		}
		return false;
	}

}