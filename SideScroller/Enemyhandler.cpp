#include "EnemyHandler.h"


CEnemyHandler::CEnemyHandler()
{

}


CEnemyHandler::~CEnemyHandler()
{
	
}

void CEnemyHandler::HandleEnemies()
{
	for (int i = 0; i < enemyVector->size(); i++)
	{
		
	}

	int size = enemyVector->size();

	for (int i = 0; i < size; i++)
	{
		if (enemyVector->at(i)->GetHP() <= 0)
		{
			if (enemyVector->at(i)->Die())
			{
				vm->DeleteObject(enemyVector->at(i));
				size--;
				i--;
			}
		}
	}
}