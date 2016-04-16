#ifndef CRATE_H
#define CRATE_H
#include "BaseObject.h"
#include "Global.h"
#include "ItemEnum.h"
class CCrate :
	public CBaseObject
{
public:
	CCrate(int x, int y);
	CCrate(int x, int y, int content);
	virtual ~CCrate();
	bool Die();
	void Init();
	
	void SetHP(int hp);
	enum animations
	{
		idle,
		death,
		numberOfAnimations
	};
	int animationData[numberOfAnimations][numberOfDataTypes];
	void TakeDamage(int dmg);
	bool IsHealthy();
	bool IsDying();
	int GetFrame();
	void SetContent(int content);
	int GetContent();
	
private:
	int content;
	bool dying;
	int timeToDie;
	int hp;
};

#endif