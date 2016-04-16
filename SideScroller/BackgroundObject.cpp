#include "BackgroundObject.h"


CBackgroundObject::CBackgroundObject(int x, int y, int _textureId, int _width, int _height, float _scrollMultiplier) : CBaseObject(x, y, _textureId, _width, _height)
{
	scrollMultiplier = _scrollMultiplier;
	renderOrder = 0;
}


CBackgroundObject::~CBackgroundObject()
{
}

bool CBackgroundObject::GetAdjustedX(int resX, int scrollX, int & result)
{
	if (pos[0] - (scrollX) <= resX)
	{
		if (pos[0] - (scrollX*scrollMultiplier) >= GetWidth() * -1)
		{
			result = pos[0] - scrollX * scrollMultiplier;
			return true;
		}
		else return false;
	}
	else return false;
}


bool CBackgroundObject::GetAdjustedY(int resY, int scrollY, int & result)
{
	if (pos[1] - (scrollY) <= resY)
	{
		if (pos[1] - (scrollY* scrollMultiplier) >= GetHeight() * -1)
		{
			result = pos[1] - scrollY *scrollMultiplier;
			return true;
		}
		else return false;
	}
	else return false;
}

int CBackgroundObject::GetFrame()
{
	return 0;
}