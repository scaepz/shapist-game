#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "BaseObject.h"
#include "PhysicsObject.h"
#include "Animate.h"
#include "Player.h"
#include <string>
#include "VectorManager.h"
#include <fstream>
#include "StaticObject.h"
#include "Enemy.h"
#include "Fluid.h"
#include "Crate.h"
#include "Chest.h"
#include "EventTriggerZone.h"
#include "Group.h"
#include <map>
using std::string;
using std::map;
class EntityFactory
{
public:
	bool CreateEntity(char id, int x, int y);
	EntityFactory(string path, CVectorManager *vm, CSpeechManager*);
	virtual ~EntityFactory();	
	
	int currentType;
	enum types
	{
		enemy,
		trap,
		crate,
		chest,
		eventTrigger,
		backgroundObject
	};
	//temporary pointer holders
	CEnemy* enemyHolder;
	CTrapObject* trapHolder;
	CCrate* crateHolder;
	CChest* chestHolder;
	CEventTrigger* eventHolder;
	CBackgroundObject* backgroundHolder;
	map<int, CGroup*>* groupMap;
private:
	string GetRow(const string & file, int row); //Digs through string and returns a string containing the row specified in int row
	
	CEnemy* CreateEnemy(const string & file, int startPoint, int x, int y);
	CTrapObject * CreateTrap(const string & file, int startpoint, int x, int y);
	CCrate * CreateCrate(const string & file, int startpoint, int x, int y);	
	CChest* CreateChest(const string & file, int startpoint, int x, int y);
	CEventTrigger* CreateEvent(const string & file, int startpoint, int x, int y);
	CBackgroundObject* CreateBackgroundObject(const string & file, int startpoint, int x, int y);
	CSpeechManager * smgr;
	CVectorManager *vm;
	string entityDataPath;
};

#endif