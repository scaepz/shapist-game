#include "FileReader.h"


CFileReader::CFileReader()
{
}


CFileReader::~CFileReader()
{
}

string CFileReader::GetRow(const string & file, int rownumber)
{
	int rowCounter = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (rowCounter == rownumber)
		{
			int counter = 0;
			int tally = 0;
			string row;
			while (true)
			{
				if (i + counter < file.size())
				{
					if (file.at(i + counter) != '\n')
					{
						counter++;
					}
					else
					{
						row = file.substr(i, counter);
						return row;
					}
				}
				else return "eof";
			}
		}
		if (file.at(i) == '\n')
		{
			rowCounter++;
		}
	}
	return "eof";
}