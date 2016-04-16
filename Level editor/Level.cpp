#include "stdafx.h"
#include "Level.h"


CLevel::CLevel(int w, int h)
{
	levelArray = new char*[w];
	for (int i = 0; i < w; ++i)
		levelArray[i] = new char[h];
	width = w;
	height = h;
}


CLevel::~CLevel()
{
	
	delete [] levelArray;
	//delete 2d array! this is not enough!
}
