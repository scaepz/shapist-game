#ifndef LEVELREADER_H
#define LEVELREADER_H

#include <string>
#include <iostream>
#include <fstream>
class CLevelReader
{
public:
	std::string levelString;
	void importLevel(std::string path);

	CLevelReader();
	virtual ~CLevelReader();


	bool NextTile();
	int currentTileNumber;
	std::string entityId;


	int tileType;
	int GetTileType();
	enum tileType
	{
		background,
		block,
		entity
	};
};

#endif