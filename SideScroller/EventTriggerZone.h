#ifndef EVENTTRIGGERZONE_H

#include "EventTrigger.h"
class CEventTriggerZone :
	public CEventTrigger
{
public:
	CEventTriggerZone(CVectorManager * vm, int id);
	virtual ~CEventTriggerZone();
	void SetArea(int x, int y, int width, int height);
	bool IsTriggered();
private:
	int pos[2];
	int width;
	int height;
};

#define EVENTTRIGGERZONE_H
#endif // !EVENTTRIGGERZONE_H
