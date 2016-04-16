#ifndef STOPCOMMAND_H
#define STOPCOMMAND_H

#include "CommandObject.h"

class CStopCommand :
	public CCommandObject
{
public:
	CStopCommand(CAnimate * _obj);
	virtual ~CStopCommand();
	bool Execute();
private:
	int staminaCost;
};

#endif