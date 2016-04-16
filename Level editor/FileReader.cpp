#include "stdafx.h"
#include "FileReader.h"


CFileReader::CFileReader()
{
}


CFileReader::~CFileReader()
{
}
BOOL CFileReader::Append(LPSTR & str, LPSTR & add)
{
	LPSTR temp;
	temp = str;

	str = new TCHAR[strlen(temp) + strlen(add) + 1];

	for (int i = 0; i < strlen(temp); i++)
	{
		str[i] = temp[i];
	}
	for (int i = strlen(temp); i < strlen(temp) + strlen(add); i++)
	{
		str[i] = add[i-strlen(temp)];
	}
	str[strlen(temp) + strlen(add)] = '\0';
	delete[] temp;
	return true;

}
TCHAR* CFileReader::CopyString(LPSTR str)
{
	LPSTR newString = new TCHAR[strlen(str) +1];
	for (int i = 0; i < strlen(str); i++)
	{
		newString[i] = str[i];
	}
	newString[strlen(str)] = '\0';
	return newString;
}
BOOL CFileReader::ReadFile2(LPSTR file, TCHAR * &content)
{
	HANDLE hFile;
	DWORD dwFileSize = 0;

	hFile = CreateFile(file,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // open only if exists
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	dwFileSize = GetFileSize(hFile, NULL);

	if (content != nullptr ^ !content ^ content != NULL) //if pointer is set
	{
		delete [] content;
	}
	content = new TCHAR[dwFileSize + 1];
	content[dwFileSize] = '\0';
	DWORD dwBytesRead;
	if (ReadFile(hFile, content, dwFileSize, &dwBytesRead, NULL))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hFile);
		return FALSE;
	}

}
BOOL CFileReader::WriteRow(LPSTR file, TCHAR * &content)
{
	HANDLE hFile;
	DWORD dwFileSize = 0;
	TCHAR newRow[2] = { '\r', '\n' };

	hFile = CreateFile(file,               // file to open
		GENERIC_WRITE,          // open for write
		FILE_SHARE_WRITE,       // share for writing
		NULL,                  // default security
		OPEN_ALWAYS,         // creates new if does not exist
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//Move cursor to end of file
	SetFilePointer(hFile, GetFileSize(hFile, NULL), NULL, FILE_BEGIN);

	DWORD dwBytesWritten;
	if (GetLastError() == ERROR_ALREADY_EXISTS && GetFileSize(hFile, NULL) != 0)
	{
		if (!WriteFile(hFile, newRow, 2, &dwBytesWritten, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}

	if (WriteFile(hFile, content, strlen(content), &dwBytesWritten, NULL))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hFile);
		return FALSE;
	}
}
BOOL CFileReader::ReplaceFileContent(LPSTR file, LPSTR content)
{
	HANDLE hFile;
	DWORD dwFileSize = 0;

	hFile = CreateFile(file,               // file to open
		GENERIC_WRITE,          // open for write
		FILE_SHARE_WRITE,       // share for writing
		NULL,                  // default security
		CREATE_ALWAYS,         // creates always -> replace old file
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DWORD dwBytesWritten;
	if (WriteFile(hFile, content, strlen(content), &dwBytesWritten, NULL))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hFile);
		return FALSE;
	}
}

BOOL CFileReader::ReplaceString(TCHAR *& str, LPSTR newContent)
{
	if (!str || str == NULL || str == nullptr) delete[] str; //if pointer is set

	str = new TCHAR[strlen(newContent) + 1];
	str[strlen(newContent)] = '\0';

	for (int i = 0; i < strlen(newContent); i++)
	{
		str[i] = newContent[i];
	}
	return TRUE;
}
BOOL CFileReader::FileExists(LPSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
BOOL CFileReader::ReadRow(LPSTR filepath, TCHAR *& file, int row)
{
	if (!ReadFile2(filepath, file))
	{
		return false;
	}
	int currentRow = 0;
	BOOL foundRow = FALSE;
	TCHAR * result = new TCHAR[512];
	int currentRowIndex = 0;

	for (int i = 0; i < strlen(file); i++)
	{
		if (currentRow == row)
		{
			foundRow = TRUE;
			if (file[i] != '\r')
			{
				result[currentRowIndex] = file[i];
				currentRowIndex++;
			}
		}
		if (file[i] == '\n') currentRow++;
		if (currentRow > row) break;
	}
	if (foundRow == TRUE)
	{
		result[currentRowIndex] = '\0';
		if (result[currentRowIndex - 1] == '\n') result[currentRowIndex - 1] = '\0';
		delete []file;

		//make new string instead of result because it is unnecessarily long
		file = new TCHAR[strlen(result)];
		for (int i = 0; i < strlen(result); i++)
		{
			file[i] = result[i];
		}
		file[strlen(result)] = '\0';

		delete []result;

		return TRUE;
	}
	else
	{
		delete[] result;
		delete [] file;
		return FALSE;
	}
}

BOOL CFileReader::RemoveRow(LPSTR file, int row)
{
	TCHAR *content;
	if (!ReadFile2(file, content))
	{
		return FALSE;
	}
	int currentRow = 0;
	BOOL foundRow = FALSE;

	if (row == 0)
	{
		foundRow = TRUE;
		while (content[0] != '\n' && content[0] != '\0')
		{
			for (int x = 0; x < strlen(content); x++)
			{
				content[x] = content[x + 1];
			}
		}
		if (content[0] == '\n')
		{
			for (int x = 0; x < strlen(content); x++)
			{
				content[x] = content[x + 1];
			}
		}

	}
	else
	for (int i = 0; i < strlen(content); i++)
	{
		if (currentRow == row)
		{
			foundRow = TRUE;
			if (content[i] != '\r')
			{
				for (int x = i; x < strlen(content); x++)
				{
					content[x] = content[x + 1];
				}
				i--;
			}
			else
			{
				break;
			}
		}
		if (content[i] == '\n') 
		{
			currentRow++;
			
			i--;
		}
		if (currentRow > row) break;
	}
	
	if (!foundRow)
	{
		delete[] content;
		return FALSE;
	};
	
	if (ReplaceFileContent(file, content))
	{
		delete[] content;
		return TRUE;
	}
	else
	{
		delete[] content;
		return FALSE;
	}
}

TCHAR* CFileReader::IntToString(int number)
{
	if (number == 0)
	{
		LPSTR str = new TCHAR[2];
		str[0] = '0';
		str[1] = '\0';
		return str;
	}
	int negative = 0;
	if (number < 0) { negative = 1; number *= -1; }
	//Get number of numbers
	int amountOfNumbers = 10;
	int dividend = 1000000000;
	while (number % dividend == number && dividend != 0)
	{
		amountOfNumbers--;
		dividend /= 10;
	}
	
	//Fill string
	LPSTR str = new TCHAR[amountOfNumbers +1 + negative];
	if (negative == 1) { str[0] = '-'; }
	for (int i = amountOfNumbers-1 + negative; i >= negative; i--)
	{
		str[i] = (number % 10) + 48; //48 is ASCII # for 0, 49 is 1 etc
		number /= 10;
	}
	str[amountOfNumbers + negative] = '\0';
	return str;
}
TCHAR* CFileReader::FloatToString(float number)
{
	int negative = 0;
	if (number < 0) { negative = 1; number *= -1; }
	int intPart = number;
	float floatPart = number - intPart; 
	LPSTR intPartStr;
	if (negative == 1)
	{
		intPartStr = new TCHAR[2];
		intPartStr[0] = '-';
		intPartStr[1] = '\0';
		LPSTR temp = IntToString(intPart);
		Append(intPartStr, temp);
		delete[] temp;
	}
	else
	{
	intPartStr = IntToString(intPart);
	}
	float dividend = 0.1f;
	while (floatPart / dividend != (int)(floatPart/dividend) ) //so hacky (loop while the division results in a non-integer)
	{
		dividend /= 10;
	}
	int ifloatPart = floatPart / dividend;
	LPSTR floatPartStr = IntToString(ifloatPart);
	
	LPSTR str = new TCHAR[strlen(intPartStr) + 1 + negative];
	if (negative == 1) str[0] = '-';
	for (int i = negative; i < strlen(intPartStr); i++)
	{
		str[i] = intPartStr[i];
	}
	str[strlen(intPartStr)] = '\0';
	LPSTR dot = new TCHAR[2];
	dot[0] = '.';
	dot[1] = '\0';
	Append(str, dot);
	delete[] dot;
	Append(str, floatPartStr);
	delete[] floatPartStr;
	delete[] intPartStr;
	return str;
}

float CFileReader::StringToFloat(LPSTR str)
{
	int dotOnIndex = strlen(str);
	int decimal = -1;
	int negative = 0;
	if (str[0] == '-')
	{
		negative = 1;
	}
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] == '.') dotOnIndex = i;
	}
	if (dotOnIndex == strlen(str)) decimal = 0;

	LPSTR intPart = new TCHAR[strlen(str) - (strlen(str) - dotOnIndex) + 1];
	intPart[strlen(str) - (strlen(str) - dotOnIndex)] = '\0';
	for (int i = negative; i < strlen(intPart); i++)
	{
		intPart[i] = str[i];
	}


	LPSTR floatPart = new TCHAR[strlen(str) - strlen(intPart)];
	floatPart[strlen(str) - strlen(intPart) + decimal] = '\0';
	for (int i = 0; i < strlen(floatPart); i++)
	{
		floatPart[i] = str[i+strlen(intPart) +1];
	}


	float number = 0;
	int multiplier = 1;
	for (int i = strlen(intPart)-1; i >= negative; i--)
	{
		number += (intPart[i] - 48) * multiplier;
		multiplier *= 10;
	}

	int divider = 1;
	for (int i = 0; i < strlen(floatPart); i++)
	{
		number += (float)(floatPart[i] - 48) / divider;
		divider *= 10;
	}
	if (negative == 1) number *= -1;
	return number;
}