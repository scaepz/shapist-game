#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H
#include "CommandObject.h"
class CAttackCommand :
	public CCommandObject
{
public:
	CAttackCommand(CAnimate * _obj);
	bool Execute();
	virtual ~CAttackCommand();
};

#endif