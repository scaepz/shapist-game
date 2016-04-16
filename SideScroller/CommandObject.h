#ifndef COMMANDOBJECT_H
#define COMMANDOBJECT_H

#include "Global.h"
#include "Animate.h"

class CCommandObject
{
public:
	static enum commands
	{
		jump,
		moveright,
		moveleft,
		stop,
		softstop,
		dash,
		attack,
		switchWeapon,
		ceaseFire,
		interact,
		slowMoveRight,
		slowMoveLeft,
		numberOfCommands
	};
	int commandId;
	CCommandObject(CAnimate * _obj);
	virtual ~CCommandObject();
	virtual bool Execute() = 0;  // Returns true if command is executed or time has run out. Returns false if command did not execute. Delete if true, keep if false.
	CAnimate * obj; // Target

};

#endif