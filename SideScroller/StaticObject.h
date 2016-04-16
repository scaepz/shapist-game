#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#include "BaseObject.h"
class CStaticObject :
	public CBaseObject
{
public:
	CStaticObject(int x, int y, int _textureId, int size);
	virtual ~CStaticObject();
};

#endif