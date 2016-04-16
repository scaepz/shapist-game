#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include "CommandObject.h"

class CJumpCommand :
	public CCommandObject
{
public:
	
	CJumpCommand(CAnimate * _obj);
	virtual ~CJumpCommand();
	bool Execute();
private:
	int tries;
	int staminaCost;
};

#endif