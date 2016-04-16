#ifndef BACKGROUNDOBJECT_H
#define BACKGROUNDOBJECT_H
#include "BaseObject.h"
class CBackgroundObject :
	public CBaseObject
{
public:
	CBackgroundObject(int x, int y, int _textureId, int _width, int _height, float scrollMultiplier);
	virtual ~CBackgroundObject();
	bool GetAdjustedX(int resX, int screenX, int & result);
	bool GetAdjustedY(int resY, int screenY, int & result);
	//animation data
	int GetFrame();
	int msPerFrame;
	int numberOfFrames;
	
private:
	float scrollMultiplier;
};

#endif