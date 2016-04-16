#ifndef CHEST_H
#define CHEST_H
#include "BaseObject.h"
#include "Global.h"

#include "WeaponsEnum.h"
class CChest : public CBaseObject
{
public:
	CChest();
	virtual ~CChest();
	int GetFrame();
	void SetContent(int);
	int GetContent();
	void SetKey(int);
	int GetKey();
	void Unlock();
	bool IsLocked();
	enum dataType
	{
		frameCountData,
		msPerFrameData,
		startFrameData,
		numberOfDataTypes
	};
	enum animations
	{
		locked,
		opened,
		numberOfAnimations
	};
	int animationData[numberOfAnimations][numberOfDataTypes];
private:
	bool closed;
	int content;
	int keyRequired;


};

#endif