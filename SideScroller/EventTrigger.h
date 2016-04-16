#ifndef EVENTTRIGGER_H
#define EVENTTRIGGER_H
#include "VectorManager.h"
#include "EventEnum.h"
class CEventTrigger
{
public:
	CEventTrigger(CVectorManager * vm, int _id);
	virtual bool IsTriggered() = 0;
	virtual int ExecuteEvent();
	int eventId;
	virtual ~CEventTrigger();

protected:
	CVectorManager *vm;
};

#endif