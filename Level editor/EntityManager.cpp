#include "stdafx.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "../SideScroller/ItemEnum.h"

CEntityManager::CEntityManager()
{
	CFileReader writer;
	writer.ReplaceString(entityFolder, "data/entities/");
}

bool CEntityManager::CreateEnemy(TCHAR*& errorMessage, LPSTR name, LPSTR textureName, int width, int height, int hp, float weight, float jumpStrength, float acceleration, float maxMoveSpeed, int loot, int animationData[7][2], int frameW, int frameH, int ai, int aiparam[4])
{
	// sanity checks
	if (name == nullptr || name == NULL || name == "") { errorMessage = "Enemy needs a name"; return false; }
	if (textureName == nullptr || textureName == NULL || textureName == "") { errorMessage = "Enemy needs a texture"; return false; }
	CTextureManager texMag;
	if (texMag.ExistsTexture(textureName) < 0) { errorMessage = "Could not find texture"; return false; }
	if (width < 0 || height < 0) { errorMessage = "Width and height must be positive"; return false; }
	if (weight < 0) { errorMessage = "Weight must be positive"; return false; }
	if (jumpStrength < 0) { errorMessage = "Jumpstr must be positive"; return false; }
	if (loot < 0 || loot > numberOfItems) { errorMessage = "Loot must be between 0 and number of items"; return false; }
	//etc

	CFileReader writer;
	LPSTR path = writer.CopyString(entityFolder);
	writer.Append(path, name);
	LPSTR txt = writer.CopyString(".txt");
	writer.Append(path, txt);
	writer.ReplaceFileContent(path, "[enemy]");
	writer.WriteRow(path, textureName);

	LPSTR sWidth = writer.IntToString(width);
	writer.WriteRow(path, sWidth);
	delete[] sWidth;

	LPSTR sHeight = writer.IntToString(height);
	writer.WriteRow(path, sHeight);
	delete[] sHeight;

	LPSTR sHp = writer.IntToString(hp);
	writer.WriteRow(path, sHp);
	delete[] sHp;


	LPSTR sWeight = writer.FloatToString(weight);
	writer.WriteRow(path, sWeight);
	delete[] sWeight;
	
	LPSTR sJumpstr = writer.FloatToString(jumpStrength);
	writer.WriteRow(path, sJumpstr);
	delete[] sJumpstr;


	LPSTR sAcceleration = writer.FloatToString(acceleration);
	writer.WriteRow(path, sAcceleration);
	delete[] sAcceleration;

	LPSTR sMaxSpeed = writer.FloatToString(maxMoveSpeed);
	writer.WriteRow(path, sMaxSpeed);
	delete[] sMaxSpeed;

	LPSTR temp = writer.IntToString(loot);
	writer.WriteRow(path, temp);
	delete[] temp;

	for (int i = 0; i < enemyAnimations; i++)
	{
		for (int x = 0; x < animDataTypes; x++)
		{
			temp = writer.IntToString(animationData[i][x]);
			writer.WriteRow(path, temp);
			delete[] temp;
		}
	}
	temp = writer.IntToString(frameW);
	writer.WriteRow(path, temp);
	delete[] temp;

	temp = writer.IntToString(frameH);
	writer.WriteRow(path, temp);
	delete[] temp;

	temp = writer.IntToString(ai);
	writer.WriteRow(path, temp);
	delete[] temp;

	for (int i = 0; i < 4; i++)
	{
		temp = writer.IntToString(aiparam[i]);
		writer.WriteRow(path, temp);
		delete[] temp;
	}
	return true;
}

CEntityManager::~CEntityManager()
{
}
