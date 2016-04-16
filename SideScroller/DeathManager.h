#ifndef DEATHMANAGER_H
#define DEATHMANAGER_H
//Crates && Items
#include "HealthBubble.h"
#include "Key.h"
#include "VectorManager.h"
#include "ItemEnum.h"
#include "Group.h"
#include <map>
class CDeathManager
{
public:
	CDeathManager(CVectorManager * vm, std::map<int, CGroup*> * _groupMap);
	~CDeathManager();
	void UpdateCrates();
	void UpdateAnimates();
private:
	std::map<int, CGroup*> * groupMap;
	CVectorManager* vm;
	vector<CCrate*>* crateVector;
	vector<CAnimate*>* animateVector;

};

#endif