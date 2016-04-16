#pragma once
#include "stdafx.h"
class CEntityManager
{
public:
	const static int enemyAnimations = 7;
	const static int animDataTypes = 2;
	bool CreateEnemy(TCHAR*& errorMessage, LPSTR name, LPSTR textureName, int width, int height, int hp, float weight, float jumpStrength, float acceleration, float maxMoveSpeed, int loot, int animationData[enemyAnimations][animDataTypes], int frameW, int frameH, int ai, int aiparam[4]);
	CEntityManager();
	virtual ~CEntityManager();
private:
	LPSTR entityFolder;


};

