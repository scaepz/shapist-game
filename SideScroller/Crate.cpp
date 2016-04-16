#include "Crate.h"


CCrate::CCrate(int x, int y) : CBaseObject(x,y)
{
	renderOrder = 3;
	ChangeAnimation(idle);
	SetHP(150);
	lookingLeft = false;
	content = nothing;
}
CCrate::CCrate(int x, int y, int _content) : CBaseObject(x, y)
{
	if (_content < numberOfItems && _content > 0)
	{
		content = _content;
	}
	else
	{
		content = nothing;
	}
	renderOrder = 3;
	ChangeAnimation(idle);
	SetHP(150);
	lookingLeft = false;
}

void CCrate::SetContent(int _content)
{
	if (_content < numberOfItems && _content > 0)
	{
		content = _content;
	}
	else
	{
		content = nothing;
	}
}
int CCrate::GetContent()
{
	return content;
}
CCrate::~CCrate()
{
}

int CCrate::GetFrame()
{
	
	if (timeSinceAnimationStart / animationData[currentAnim][msPerFrameData] < animationData[currentAnim][frameCountData])
	{
		return animationData[currentAnim][startFrameData] + timeSinceAnimationStart / animationData[currentAnim][msPerFrameData];
	}
	else
	{
		ResetTime();
		return GetFrame();
	}
}

void CCrate::TakeDamage(int dmg)
{
	hp -= dmg;
}

bool CCrate::IsHealthy()
{
	if (hp > 0)
	{
		return true;
	}
	else return false;
}

bool CCrate::Die()
{
	if (!IsDying())
	{
		ChangeAnimation(death);
		dying = true;
	}
	timeToDie -= g_time;
	if (timeToDie > 0)
	{
		return false;
	}
	else return true;
}

bool CCrate::IsDying()
{
	return dying;
}

void CCrate::SetHP(int _hp)
{
	hp = _hp;
}
