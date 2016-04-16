#pragma once
#include "BaseObject.h"
class CDevLaser :
	public CBaseObject
{
public:
	CDevLaser(int x, int y, int _textureId, int width, int height, float degrees);
	int GetFrame();
	bool UpdateLife();
	int lifeInMs;
	virtual ~CDevLaser();
	void SetRotation(float rotation);
};

