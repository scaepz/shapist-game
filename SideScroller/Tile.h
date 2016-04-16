#ifndef TILE_H
#define TILE_H
#include <vector>

class CTile
{
public:
	int pos[2];
	bool visible;
	bool touchable;
	void CalculateProperties(const std::vector<std::vector<CTile*>> & tileVector);
	int texture;
	enum textures
	{
		upperLeftCorner,
		upperRightCorner,
		lowerLeftCorner,
		lowerRightCorner,

		floor,
		roof,

		horizontalThin,
		verticalThin,

		leftWall,
		rightWall,

		center,

		thumbUp,
		thumbLeft,
		thumbRight,
		thumbDown,

		upperLeftInwardCorner,
		upperRightInwardCorner,
		lowerLeftInwardCorner,
		lowerRightInwardCorner,

		rightWallUpperLeftInwardCorner,
		rightWallLowerLeftInwardCorner,
		leftWallLowerRightInwardCorner,
		leftWallUpperRightInwardCorner,

		rightWallTwoCorners,
		leftWallTwoCorners,
		roofTwoCorners,
		floorTwoCorners,


		floorLeftInwardCorner,
		floorRightInwardCorner,
		roofLeftInwardCorner,
		roofRightInwardCorner,

		solo,

		twoInwardCornersUp,
		twoInwardCornersDown,
		twoInwardCornersRight,
		twoInwardCornersLeft,

		upperLeftCornerThin,
		upperRightCornerThin,
		lowerLeftCornerThin,
		lowerRightCornerThin,

		

		threeInwardCornersUpperLeft,
		threeInwardCornersUpperRight,
		threeInwardCornersLowerLeft,
		threeInwardCornersLowerRight,

		fourInwardCorners,

		thinCornerUpperLeftLowerRight,
		thinCornerLowerLeftUpperRight

	};	
	CTile(int x, int y);
	virtual ~CTile();

private:
	bool CompareTileArrays(bool tileSurroundings[][3], int _template[][3]);
};

#endif