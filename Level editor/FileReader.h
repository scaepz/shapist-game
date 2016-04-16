#pragma once
#include "stdafx.h"
class CFileReader
{
public:
	BOOL ReadFile2(LPSTR filepath, TCHAR *&);
	BOOL ReadRow(LPSTR filepath, TCHAR *&, int row);
	BOOL WriteRow(LPSTR file, TCHAR *& text);
	BOOL ReplaceFileContent(LPSTR file, LPSTR text);
	BOOL RemoveRow(LPSTR file, int row);
	BOOL ReplaceString(TCHAR *&, LPSTR newContent);
	BOOL FileExists(LPSTR szPath);
	BOOL Append(LPSTR & str, LPSTR & add);
	TCHAR* CopyString(LPSTR str);

	//Dont ask
	TCHAR* IntToString(int);
	TCHAR* FloatToString(float);
	float StringToFloat(LPSTR);

	CFileReader();
	virtual ~CFileReader();
};