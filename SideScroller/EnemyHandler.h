#ifndef EnemyHANDLER_H
#define EnemyHANDLER_H

#include <vector>

#include "Enemy.h"
#include "AI.h"
#include "SimpleTestAI.h"
#include "Player.h"


class CEnemyHandler
{
public:
	CEnemyHandler();
	virtual ~CEnemyHandler();
	void HandleEnemies();
	
private:
	CPlayer * player;
	CVectorManager *vm;
	vector<vector<CTile*>> * tileVector;
	vector<CCommandObject*>* commandVector;
	vector<CAI*> AIVector;
	vector<CEnemy*>* enemyVector;
};

#endif