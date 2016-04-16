#include "Chest.h"


CChest::CChest()
{
	length = 64;
	height = 64;
	frameSizeX = 64;
	frameSizeY = 64;
	renderOrder = 1;
}

void CChest::Unlock()
{
	closed = false;
}

CChest::~CChest()
{
}


bool CChest::IsLocked()
{
	return closed;
}

int CChest::GetFrame()
{
	if (closed)
	{
		return 0;
	}
	else return 1;
}

void CChest::SetContent(int weaponId)
{
	content = weaponId;
}
int CChest::GetContent()
{
	return content;
}
void CChest::SetKey(int key)
{
	keyRequired = key;
}
int CChest::GetKey()
{
	return keyRequired;
}