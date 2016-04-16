#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include <string>

#include "vector2.h"
class CBaseObject
{
public:
	CBaseObject(int x, int y,int _textureId, int size);
	CBaseObject(int x, int y, int _textureId, int length, int height);
	CBaseObject(int x, int y);
	CBaseObject();
	virtual ~CBaseObject();

	//animation, rendering
	int textureId;
	virtual int GetFrame() = 0;
	int frameSizeX;
	int frameSizeY;
	int timeSinceAnimationStart = 0;
	virtual void UpdateTime(int time);
	void ResetTime();
	void AddTime(int ms);
	int renderOrder = 2; //0-5, 5 in front
	virtual void ChangeAnimation(int animation);
	int currentAnim;
	enum dataType
	{
		frameCountData,
		msPerFrameData,
		startFrameData,
		numberOfDataTypes
	};

	virtual void TakeDamage(int dmg);
	virtual bool IsHealthy();
	virtual bool Die();
	
	//pos, size, rotation
	int length;
	int height;
	double pos[2];
	virtual int GetWidth();
	virtual int GetHeight();
	virtual int GetX();
	virtual int GetY();
	float GetRotation();
	float GetRealRotation();
	virtual void SetRotation(float rotation);
	float rotation = 0; //rotation to show
	float realRotation = 0;
	bool lookingLeft = false;
	bool ableToPassThroughEntities = false;
};

#endif