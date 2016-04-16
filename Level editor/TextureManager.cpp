#include "stdafx.h"
#include "TextureManager.h"


CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
}
BOOL CTextureManager::GetTexturePath(LPSTR name, TCHAR * & path)
{
	int index = ExistsTexture(name);
	if (index < 0) return FALSE;
	CFileReader reader;
	return reader.ReadRow(textureFile, path ,index + 1);
}

int CTextureManager::ExistsTexture(LPSTR name)
{
	int i = 0;
	LPSTR row;
	CFileReader reader;
	while (reader.ReadRow(textureFile, row, i * 2))
	{
		if (row[strlen(row) - 1] == '\r' || row[strlen(row) - 1] == '\n')
		{
			row[strlen(row) - 1] = '\0';
		}
		if (strlen(name) == strlen(row))
		{
			bool different = false;
			for (int x = 0; x < strlen(name); x++)
			{
				if (row[x] != name[x])
				{
					different = true;
					break;
				}
			}
			if (!different)
			{
				//delete [] row; crashes program - why?
				return i*2;
			}
		}
		i++;
	}
	//delete[] row; crashes program - why?
	return -1;
}

BOOL CTextureManager::GetNextTextureName(TCHAR *& _name)
{
	CFileReader reader;
	TCHAR * name;
	if (reader.ReadRow(textureFile, name, currentRow))
	{
		_name = name;
		currentRow += 2;
		return true;
	}
	ResetTextureCursor();
	return false;
}
void CTextureManager::ResetTextureCursor()
{
	currentRow = 0;
}

BOOL CTextureManager::AddTexture(LPSTR name, LPSTR path)
{
	CFileReader writer;
	if (ExistsTexture(name) < 0)
	{
		if (writer.WriteRow(textureFile, name))
		{
			return writer.WriteRow(textureFile, path);
		}
		else return FALSE;
	}
	else return FALSE;
}
BOOL CTextureManager::RemoveTexture(LPSTR name)
{
	int index = ExistsTexture(name);
	if (index < 0) return FALSE;
	else
	{
		CFileReader writer;
		if (writer.RemoveRow(textureFile, index))
		{
			return writer.RemoveRow(textureFile, index);
		}
		else return FALSE;
	}
}