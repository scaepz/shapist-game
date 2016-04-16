#include "Tile.h"


CTile::CTile(int x, int y)
{
	pos[0] = x;
	pos[1] = y;
	touchable = true;
}


CTile::~CTile()
{
}

void CTile::CalculateProperties(const std::vector<std::vector<CTile*>> & tileVector)
{
	bool surroundings[3][3];
	surroundings[1][1] = true;
	bool outSideSolid = true;

	//The forest of Ifelse fills the array "surroundings" with the neighbors of this tile for easier comparisons. True is block, false is no block
	if (pos[1] != 0)
	{
		if (pos[0] != 0)
		{
			if (tileVector[pos[1] - 1][pos[0] - 1] == nullptr)
			{
				surroundings[0][0] = false;
			}
			else
			{
				surroundings[0][0] = true;
			}
		}
		else
		{
			surroundings[0][0] = outSideSolid;
		}
		///////////
		if (tileVector[pos[1] - 1][pos[0]] == nullptr)
		{
			surroundings[1][0] = false;
		}
		else
		{
			surroundings[1][0] = true;
		}
		//////////
		if (pos[0] != tileVector.at(pos[1]).size() - 1)
		{
			if (tileVector[pos[1] - 1][pos[0] + 1] == nullptr)
			{
				surroundings[2][0] = false;
			}
			else
			{
				surroundings[2][0] = true;
			}

		}
		else
		{
			surroundings[2][0] = outSideSolid;
		}
	}
	else
	{
		surroundings[0][0] = outSideSolid;
		surroundings[1][0] = outSideSolid;
		surroundings[2][0] = outSideSolid;
	}
	//////////
	if (pos[0] != 0)
	{
		if (tileVector[pos[1]][pos[0] - 1] == nullptr)
		{
			surroundings[0][1] = false;
		}
		else
		{
			surroundings[0][1] = true;
		}
	}
	else
	{
		surroundings[0][1] = outSideSolid;
	}
	////////////
	if (pos[0] != tileVector.at(pos[1]).size() - 1)
	{
		if (tileVector[pos[1]][pos[0] + 1] == nullptr)
		{
			surroundings[2][1] = false;
		}
		else
		{
			surroundings[2][1] = true;
		}
	}
	else
	{
		surroundings[2][1] = outSideSolid;
	}
	////////
	if (pos[1] != tileVector.size() - 1)
	{
		if (pos[0] != 0)
		{
			if (tileVector[pos[1] + 1][pos[0] - 1] == nullptr)
			{
				surroundings[0][2] = false;
			}
			else
			{

				surroundings[0][2] = true;
			}
		}
		else
		{
			surroundings[0][2] = outSideSolid;
		}

		////////
		if (tileVector[pos[1] + 1][pos[0]] == nullptr)
		{
			surroundings[1][2] = false;
		}
		else
		{
			surroundings[1][2] = true;
		}
		///////
		if (pos[0] != tileVector.at(pos[1]).size() - 1)
		{
			if (tileVector[pos[1] + 1][pos[0] + 1] == nullptr)
			{
				surroundings[2][2] = false;
			}
			else
			{
				surroundings[2][2] = true;
			}
		}
		else
		{
			surroundings[2][2] = outSideSolid;
		}


		///////

	}
	else
	{
		surroundings[0][2] = outSideSolid;
		surroundings[1][2] = outSideSolid;
		surroundings[2][2] = outSideSolid;
	}

	//here follows array comparisons to find out block type
	//the center of the array, which is always 1, is the current tile
	//we check its neighbours to find out what texture we should use
	//1 means block, 0 means no-block
	//2 means that this block doesn't matter and won't be taken into account in the comparison.

	bool foundMatch = false;

	////////////////////////////
	{
		// upperLeftCorner
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 0, 1, 1 },
			{ 2, 1, 1 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperLeftCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		//upperRightCorner
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 0 },
			{ 1, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperRightCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		//lowerLeftCorner
		int temp[3][3] =
		{
			{ 2, 1, 1 },
			{ 0, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerLeftCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		//lowerRightCorner
		int temp[3][3] =
		{
			{ 1, 1, 2 },
			{ 1, 1, 0 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerRightCorner;
			foundMatch = true;
		}
	}

	///////////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = floor;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = roof;
			foundMatch = true;
		}
	}

	//////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = horizontalThin;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 2 },
			{ 0, 1, 0 },
			{ 2, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = verticalThin;
			foundMatch = true;
		}
	}
	//////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thinCornerUpperLeftLowerRight;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thinCornerLowerLeftUpperRight;
			foundMatch = true;
		}
	}
	/////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 1 },
			{ 0, 1, 1 },
			{ 2, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = leftWall;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 2 },
			{ 1, 1, 0 },
			{ 1, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = rightWall;
			foundMatch = true;
		}
	}
	/////////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			touchable = false;
			texture = center;
			foundMatch = true;
		}
	}
	//////////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 0, 1, 0 },
			{ 2, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thumbUp;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 0, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thumbRight;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 0 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thumbLeft;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 2 },
			{ 0, 1, 0 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = thumbDown;
			foundMatch = true;
		}
	}
	/////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperLeftInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperRightInwardCorner;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerLeftInwardCorner;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerRightInwardCorner;
			foundMatch = true;
		}
	}
	/////////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 2 },
			{ 1, 1, 0 },
			{ 1, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = rightWallUpperLeftInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 2 },
			{ 1, 1, 0 },
			{ 0, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = rightWallLowerLeftInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 1 },
			{ 0, 1, 1 },
			{ 2, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = leftWallLowerRightInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 0 },
			{ 0, 1, 1 },
			{ 2, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = leftWallUpperRightInwardCorner;
			foundMatch = true;
		}
	}
	/////////////

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 2 },
			{ 1, 1, 0 },
			{ 0, 1, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = rightWallTwoCorners;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 1, 0 },
			{ 0, 1, 1 },
			{ 2, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = leftWallTwoCorners;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = floorTwoCorners;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = roofTwoCorners;
			foundMatch = true;
		}
	}
	////////////

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 0, 1, 1 },
			{ 1, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = roofLeftInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = roofRightInwardCorner;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = floorLeftInwardCorner;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 1 },
			{ 1, 1, 0 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = floorRightInwardCorner;
			foundMatch = true;
		}
	}
	///////////
	if (!foundMatch)
	{
		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 0, 1, 0 },
			{ 2, 0, 2 }

		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = solo;
			foundMatch = true;
		}
	}
	//////////////
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 1, 1 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = twoInwardCornersUp;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = twoInwardCornersDown;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = twoInwardCornersRight;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = twoInwardCornersLeft;
			foundMatch = true;
		}
	}

	//////////////////////

	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 0, 1, 1 },
			{ 2, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperLeftCornerThin;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 2, 0, 2 },
			{ 1, 1, 0 },
			{ 0, 1, 2 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = upperRightCornerThin;
			foundMatch = true;
		}
	}

	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 2, 1, 0 },
			{ 0, 1, 1 },
			{ 2, 0, 2 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerLeftCornerThin;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 2 },
			{ 1, 1, 0 },
			{ 2, 0, 2 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = lowerRightCornerThin;
			foundMatch = true;
		}
	}

	//////////////////////
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = threeInwardCornersUpperLeft;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = threeInwardCornersUpperRight;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = threeInwardCornersLowerLeft;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 0, 1, 1 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = threeInwardCornersUpperLeft;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{

		int temp[3][3] =
		{
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		};
		if (CompareTileArrays(surroundings, temp))
		{
			texture = fourInwardCorners;
			foundMatch = true;
		}
	}
	if (!foundMatch)
	{
		texture = center;
	}


}
bool CTile::CompareTileArrays(bool tileSurroundings[][3], int _template[][3])
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (_template[y][x] != 2)
			{
				if (_template[y][x] == 1 && tileSurroundings[x][y] == false)
				{
					return false;
				}
				else if (_template[y][x] == 0 && tileSurroundings[x][y] == true)
				{
					return false;
				}
			}
		}
	}
	return true;
}