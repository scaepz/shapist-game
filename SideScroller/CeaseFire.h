#ifndef CEASEFIRE_H
#define CEASEFIRE_H
#include "CommandObject.h"
class CCeaseFire :
	public CCommandObject
{
public:
	CCeaseFire(CAnimate * obj);
	virtual ~CCeaseFire();
	bool Execute();
};

#endif