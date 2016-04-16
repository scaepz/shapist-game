#include "EventTrigger.h"
#include "Global.h"

CEventTrigger::CEventTrigger(CVectorManager * _vm, int _id)
{
	eventId = _id;
	vm = _vm;
}


CEventTrigger::~CEventTrigger()
{
}

int CEventTrigger::ExecuteEvent()
{
	switch (eventId)
	{
	case levelComplete:
		return eventId;
	default:
		return -1;
		break;
	}
	return -1;
}