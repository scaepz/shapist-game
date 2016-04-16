#pragma once
#include "FileReader.h"
#include <string>
using namespace std;
class CTextureManager
{
public:
	CTextureManager();
	BOOL AddTexture(LPSTR name, LPSTR path);
	int ExistsTexture(LPSTR name);
	BOOL RemoveTexture(LPSTR name);
	BOOL GetTexturePath(LPSTR name, TCHAR * & path);
	virtual ~CTextureManager();
	BOOL GetNextTextureName(TCHAR *& name);
	void ResetTextureCursor();
private:
	int currentRow = 0;
	LPSTR textureFile = "data/textures/texturelist.txt";
};

