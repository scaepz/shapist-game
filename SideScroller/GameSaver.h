#ifndef GAMESAVER_H
#define GAMESAVER_H

#include "VectorManager.h"
#include <fstream>
#include <string>
using std::string;
using std::to_string;
class CGameSaver
{
public:
	CGameSaver(CVectorManager * vm, string saveName, int levelId);
	virtual ~CGameSaver();
	void SaveGame();
	CPlayer * LoadGame(int x, int y, int textureId, int size);
private:
	string GetRow(const string & file, int rowNumber);
	CVectorManager * vm;
	string saveName;
	int levelId;
};

#endif