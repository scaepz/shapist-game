#include "GroupManager.h"
#include "AIs.h"

CGroupManager::CGroupManager(std::map<int, CGroup*> * _groupMap, CVectorManager * _vm)
{
	groupMap = _groupMap;
	vm = _vm;
	CRetreatingAI * retreating = new CRetreatingAI(vm);
	AIVector.push_back(retreating);
	CAggressiveAI * agg = new CAggressiveAI(vm);
	AIVector.push_back(agg);
	CRangedAI * ranged = new CRangedAI(vm);
	AIVector.push_back(ranged);
	CCautiousRangedAI * catRanged = new CCautiousRangedAI(vm);
	AIVector.push_back(catRanged);

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
	for (std::map<int, CGroup*>::iterator it = groupMap->begin(); it != groupMap->end(); ++it)
		it->second->Update();
}