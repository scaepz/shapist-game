#include "LevelReader.h"


CLevelReader::CLevelReader()
{
	currentTileNumber = 0;
}

void CLevelReader::importLevel(std::string path)
{

	std::fstream reader(path);
	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		levelString += temp;
	}
	reader.close();
	levelString = levelString.substr(0, levelString.size() - 1);
}


bool CLevelReader::NextTile()
{
	if (levelString.size() > currentTileNumber * 2)
	{
		currentTileNumber++;
		return true;
	}
	else return false;
}

int CLevelReader::GetTileType()
{
	std::string temp = levelString.substr((currentTileNumber-1) * 2, 1);
	
	if (temp.at(0) == '0')
	{
		return 0;
	}
	else if (temp.at(0) == '1')
	{
		return 1;
	}
	else if (temp == "X")
	{
		return -1;
	}
	else if (temp.at(0) == 'a')
	{
		return 3;
	}
	else
	{
		entityId = temp;
		return 2;
	}
}

CLevelReader::~CLevelReader()
{
}
