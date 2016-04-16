#include "BaseObject.h"

CBaseObject::CBaseObject()
{

}
CBaseObject::CBaseObject(int x, int y, int _textureId, int _size)
{
	pos[0] = x;
	pos[1] = y;
	textureId = _textureId;
	length = _size;
	height = length;
}
CBaseObject::CBaseObject(int x, int y, int _textureId, int _width, int _height)
{
	pos[0] = x;
	pos[1] = y;
	textureId = _textureId;
	length = _width;
	height = _height;
}
CBaseObject::CBaseObject(int x, int y)
{
	pos[0] = x;
	pos[1] = y;
	textureId = 3;
	length = 32;
	height = length;
}

void CBaseObject::TakeDamage(int dmg)
{
	//not all baseobjects need it, but most can use it
}
void CBaseObject::ChangeAnimation(int animation)
{
	currentAnim = animation;
}
void CBaseObject::UpdateTime(int time)
{
	AddTime(time);
}
bool CBaseObject::IsHealthy()
{
	return true;
}
bool CBaseObject::Die()
{
	return false;
}
CBaseObject::~CBaseObject()
{
	textureId = 3;
	pos[0] = 0;
	pos[1] = 0;
	height = 32;
	length = height;
}

void CBaseObject::AddTime(int ms)
{
	timeSinceAnimationStart += ms;
}

void CBaseObject::ResetTime()
{
	timeSinceAnimationStart = 0;
}

int CBaseObject::GetHeight()
{
	return height;
}

int CBaseObject::GetWidth()
{
	return length;
}

int CBaseObject::GetX()
{
	return pos[0];
}

int CBaseObject::GetY()
{
	return pos[1];
}

float CBaseObject::GetRotation()
{
	return rotation;
}

float CBaseObject::GetRealRotation()
{
	return realRotation;
}

void CBaseObject::SetRotation(float _rotation)
{
	rotation = _rotation;
	realRotation = _rotation;
}