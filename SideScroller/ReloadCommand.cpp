#include "ReloadCommand.h"


CReloadCommand::CReloadCommand(CAnimate * _obj) : CCommandObject(_obj)
{
}


CReloadCommand::~CReloadCommand()
{
}

bool CReloadCommand::Execute()
{
	obj->Reload();
	return true;
}