#include "AttackCommand.h"


CAttackCommand::CAttackCommand(CAnimate * _obj) : CCommandObject(_obj)
{
	commandId = attack;
}

CAttackCommand::~CAttackCommand()
{

}

bool CAttackCommand::Execute()
{
	return (obj->Attack());
}