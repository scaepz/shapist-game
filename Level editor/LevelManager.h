#pragma once
#include "Level.h"
#include "FileReader.h"
#include "stdafx.h"
class CLevelManager
{
public:
	CLevelManager();
	virtual ~CLevelManager();
	BOOL Save(LPSTR name);
	CLevel * level;
	BOOL Load(LPSTR name);
	void New(int w, int h, bool fill);
	LPSTR currentName;
private:
	LPCSTR levelPath = "data/levels/";
};

