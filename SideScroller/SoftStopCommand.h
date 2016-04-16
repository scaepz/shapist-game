#ifndef SOFTSTOPCOMMAND_H
#define SOFTSTOPCOMMAND_H

#include "CommandObject.h"
class CSoftStopCommand :
	public CCommandObject
{
public:
	bool Execute();
	CSoftStopCommand(CAnimate* obj);
	virtual ~CSoftStopCommand();
};

#endif