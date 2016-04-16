#include "EventManager.h"


CEventManager::CEventManager()
{

}


CEventManager::~CEventManager()
{
	while (triggerVector.size() > 0)
	{
		delete triggerVector.at(0);
		triggerVector.erase(triggerVector.begin());
	}
}

void CEventManager::SetArtistPointer(CArtist* a)
{
	artist = a;
}

void CEventManager::HandleEvents()
{
	if (debugPause)
	{
		int a = 0;
		a++;
	}


	for (int i = 0; i < triggerVector.size(); i++)
	{
		if (triggerVector[i]->IsTriggered())
		{
			int message = triggerVector[i]->ExecuteEvent();

			switch (message)
			{
			case levelComplete:
				artist->FadeOut(2000);
				fadeTimeLeftInMs = 2000;
				fadeReason = levelComplete;
				break;
			default:
				break;
			}

			delete triggerVector[i];
			triggerVector.erase(triggerVector.begin() + i);
			i--;
		}
	}

	//handle fade event
	if (fadeTimeLeftInMs > 0)
	{
		fadeTimeLeftInMs -= g_time;
		if (fadeTimeLeftInMs <= 0 && fadeReason == levelComplete)
		{
			gameState = 1; //gamestate 1 means the game loop will exit and next level will be loaded
		}

	}
}

void CEventManager::AddEventTrigger(CEventTrigger* e)
{
	triggerVector.push_back(e);
}