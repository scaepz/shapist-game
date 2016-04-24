#include "GroupManager.h"
#include "AIs.h"

CGroupManager::CGroupManager(std::map<int, CGroup*> * _groupMap, CVectorManager * _vm)
{
	groupMap = _groupMap;
	vm = _vm;
	CAI * ai = new CAI(vm);
	AIVector.push_back(ai);
	CMeleeAI *meleeAi = new CMeleeAI(vm);
	AIVector.push_back(meleeAi);

	for (std::map<int, CGroup*>::iterator it = groupMap->begin(); it != groupMap->end(); ++it)
	{
		it->second->AIVector = &AIVector;
		it->second->Init();
	}
}

CGroupManager::~CGroupManager()
{
}

void CGroupManager::HandleGroups()
{
	for (auto it = groupMap->begin(); it != groupMap->end();)
	{
		if (it->second->Update())
		{
			delete it->second;
			groupMap->erase(it++);	
		}
		else
		{
			++it;
		}
	}
}