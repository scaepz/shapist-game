#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "EventTrigger.h"
#include "Artist.h"
#include <vector>
class CEventManager
{
public:
	void HandleEvents();
	CEventManager();
	void AddEventTrigger(CEventTrigger*);
	virtual ~CEventManager();
	void SetArtistPointer(CArtist*);
private:
	CArtist * artist;
	std::vector<CEventTrigger*> triggerVector;

	//fade event
	int fadeTimeLeftInMs;
	int fadeReason;

};

#endif