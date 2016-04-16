#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H
#include "CommandObject.h"
class CMoveCommand :
	public CCommandObject
{
public:
	CMoveCommand(CAnimate * _obj, int _dir);
	virtual ~CMoveCommand();
	bool Execute();
	int dir;
	bool airMovedOnce;
	static enum direction
	{
		left,
		right
	};
};

#endif