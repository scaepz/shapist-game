#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H
#include <map>
#include "VectorManager.h"
#include "Group.h"
class CGroupManager
{
public:
	void HandleGroups();
	CGroupManager(std::map<int, CGroup*> * _groupMap, CVectorManager * _vm);
	virtual ~CGroupManager();
private:
	std::vector<CAI*> AIVector;
	std::map<int, CGroup*> *groupMap;
	CVectorManager * vm;
};

#endif