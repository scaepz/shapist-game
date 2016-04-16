#ifndef RELOADCOMMAND_H
#define RELOADCOMMAND_H
#include "CommandObject.h"
class CReloadCommand :
	public CCommandObject
{
public:
	bool Execute();
	CReloadCommand(CAnimate * _obj);
	virtual ~CReloadCommand();
};
#endif // !RELOADCOMMNAND_H


