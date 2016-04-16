#include "stdafx.h"
#include "LevelManager.h"


CLevelManager::CLevelManager()
{
	level = nullptr;
}


CLevelManager::~CLevelManager()
{
}

void CLevelManager::New(int w, int h, bool fill)
{
	if (level != nullptr) delete level;
	level = new CLevel(w, h);
	char defaultChar = '0';
	if (fill)
	{
		defaultChar = '1';
	}
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			level->levelArray[x][y] = defaultChar;
		}
	}
}

BOOL CLevelManager::Save(LPSTR name)
{
	CFileReader reader;
	LPSTR levelFile = new TCHAR[level->height*(level->width+1) + 1];
	levelFile[level->height*(level->width+1)] = '\0';
	int i = 0;
	for (int y = 0; y < level->height; y++)
	{
		for (int x = 0; x < level->width; x++)
		{
			levelFile[i] = level->levelArray[x][y];
			i++;
		}
		levelFile[i] = 'X';
		i++;
	}
	LPSTR path = new TCHAR[strlen(levelPath)+strlen(name)+strlen(".txt") + 1];
	strcpy(path,levelPath);
	strcat(path, name);
	strcat(path, ".txt");
	if (reader.ReplaceFileContent(path, levelFile))
	{
		delete[] path;
		path = new TCHAR[strlen(levelPath) + strlen("levelList.txt") + 1];
		strcpy(path, levelPath);
		strcat(path, "levelList.txt");
		reader.WriteRow(path, name);
		delete[] path;
		delete[] levelFile;
		
		return TRUE;
	}
	else
	{
		delete[] path;
		delete[] levelFile;
		return FALSE;
	}
}

BOOL CLevelManager::Load(LPSTR name)
{
	return FALSE;
}