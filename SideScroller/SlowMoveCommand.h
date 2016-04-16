#ifndef SLOWMOVECOMMAND_H
#define SLOWMOVECOMMAND_H
#include "MoveCommand.h"
class CSlowMoveCommand :
	public CCommandObject
{
public:
	CSlowMoveCommand(CAnimate * _obj, int _dir);
	bool Execute();
	int dir;
	bool airMovedOnce;
	static enum direction
	{
		left,
		right
	};
	virtual ~CSlowMoveCommand();
};

#endif