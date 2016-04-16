#pragma once
#include "stdafx.h"
class CLevel
{
public:
	CLevel(int w, int h);
	virtual ~CLevel();
	char** levelArray;
	int width;
	int height;
};

