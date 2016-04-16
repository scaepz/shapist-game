#include "KickBack.h"


CKickBack::CKickBack()
{

}
void CKickBack::SetKickBack(int x, int y, int rot, int _time)
{
	relativeX = x;
	relativeY = y;
	relativeRotation = rot;
	time = _time;
	if (time == 0)
	{
		time = 1;
	}
}

CKickBack::~CKickBack()
{
}

void CKickBack::NewKickback()
{
	timeUntilNormal = time;
}

void CKickBack::Update(int time)
{
	if (timeUntilNormal > 0)
	{
		timeUntilNormal -= time;
	}
	else { timeUntilNormal = 0; };
}

int CKickBack::GetX()
{
	if (timeUntilNormal > 0)
	{
		return relativeX * (float)((float)timeUntilNormal / (float)time);
	}
	else return 0;
}
int CKickBack::GetY()
{
	if (timeUntilNormal > 0)
	{
		return relativeY * (float)((float)timeUntilNormal / (float)time);
	}
	else return 0;
}
int CKickBack::GetRotation()
{
	if (timeUntilNormal > 0)
	{
		return relativeRotation * (float)((float)timeUntilNormal / (float)time);
	}
	else return 0;
}