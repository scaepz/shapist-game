#ifndef DASHCOMMAND_H
#define DASHCOMMAND_H
#include "CommandObject.h"
class CDashCommand :
	public CCommandObject
{
public:
	//player only version of dash
	enum directionEnum
	{
		downRight,
		downLeft,
		upLeft,
		upRight
	};

	bool Execute();

	CDashCommand(CAnimate*, int dirX, int dirY);
	virtual ~CDashCommand();
private:
	int staminaCost;
	int duration;
	double velocityIncrease[2];
	int direction;
	bool exed;
};

#endif