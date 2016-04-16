#include "AIHandler.h"
#include <iostream>
#include "SDL.h"

CAIHandler::CAIHandler(CVectorManager * _vm)
{
	vm = _vm;
	CRetreatingAI * retreating = new CRetreatingAI(vm);
	AIVector.push_back(retreating);
	CAggressiveAI * agg = new CAggressiveAI(vm);
	AIVector.push_back(agg);
	CRangedAI * ranged = new CRangedAI(vm);
	AIVector.push_back(ranged);
	CCautiousRangedAI * catRanged = new CCautiousRangedAI(vm);
	AIVector.push_back(catRanged);

	float pos = 0;
	float vel = 0;
	//while (true)
	{
	//	SDL_Delay(10);
	//	ranged->GetEnemyCrosshairPlacement(&pos, &vel, 180);
	//		std::cout << pos << " at " << vel << std::endl;
	}
}


CAIHandler::~CAIHandler()
{
	while (AIVector.size() > 0)
	{
		delete AIVector.at(0);
		AIVector.erase(AIVector.begin());
	}
}

void CAIHandler::HandleAI()
{
	std::vector<CEnemy*> * enemyVector = vm->GetEnemyVector();
	for (int i = 0; i < enemyVector->size(); i++)
	{
			AIVector[enemyVector->at(i)->AI]->MakeDecision(enemyVector->at(i));
	}
}
