#include "CollisionDetector.h"

#include "Global.h"

CCollisionDetector::CCollisionDetector()
{
}


CCollisionDetector::~CCollisionDetector()
{
}
bool CCollisionDetector::Collides(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	if (debugPause)
	{
		int a = 0;
		a++;
	}
	return (x1 < x2 + w2 &&
		x1 + w1 > x2 &&
		y1 < y2 + h2 &&
		h1 + y1 > y2);
}
bool CCollisionDetector::Collides(CBaseObject * obj1, CBaseObject * obj2)
{
	return (obj1->GetX() < obj2->GetX() + obj2->GetWidth() &&
		obj1->GetX() + obj1->GetWidth() > obj2->GetX() &&
		obj1->GetY() < obj2->GetY() + obj2->GetHeight() &&
		obj1->GetHeight() + obj1->GetY() > obj2->GetY());
}

bool CCollisionDetector::Collides(CPhysicsObject * obj, const CTile * tile, int standardTileSize)
{
	
		if (tile != nullptr)
		{
			return (obj->pos[0] < tile->pos[0] * standardTileSize + standardTileSize &&
				obj->pos[0] + obj->GetWidth() > tile->pos[0] * standardTileSize  &&
				obj->pos[1] < tile->pos[1] * standardTileSize + standardTileSize &&
				obj->GetHeight() + obj->pos[1] > tile->pos[1] * standardTileSize);
		}
		else return false;
	

}
bool CCollisionDetector::RotatedCollides(CBaseObject * rObj, CBaseObject * obj, int x0, int y0)
{
	/*p'x = cos(theta) * (px-x0) - sin(theta) * (py-y0) + x0
	p'y = sin(theta) * (px-x0) + cos(theta) * (py-y0) + y0

	where:
	px, py = coordinate of the point
	y0, x0, = centre of rotation
	theta = angle of rotation
	*/

	/////////
	// Cornernumbers in array
	//
	//	   0 ___ 1
	//		|	|
	//	  3 L___I 2


	int degrees = rObj->GetRotation();
	int rotatedRect[4][2];
	//rotate rect
	rotatedRect[0][0] = cos(degrees) * (rObj->GetX() - x0) - sin(degrees) * (rObj->GetY() - y0) + x0;
	rotatedRect[0][1] = sin(degrees) * (rObj->GetX() - x0) + cos(degrees) * (rObj->GetY() - y0) + y0;
	rotatedRect[1][0] = cos(degrees) * (rObj->GetX() + rObj->GetWidth() - x0) - sin(degrees) * (rObj->GetY() - y0) + x0;
	rotatedRect[1][1] = sin(degrees) * (rObj->GetX() + rObj->GetWidth() - x0) + cos(degrees) * (rObj->GetY() - y0) + y0;
	rotatedRect[2][0] = cos(degrees) * (rObj->GetX() + rObj->GetWidth() - x0) - sin(degrees) * (rObj->GetY()+rObj->GetHeight() - y0) + x0;
	rotatedRect[2][1] = sin(degrees) * (rObj->GetX() + rObj->GetWidth() - x0) + cos(degrees) * (rObj->GetY()+rObj->GetHeight() - y0) + y0;
	rotatedRect[3][0] = cos(degrees) * (rObj->GetX() - x0) - sin(degrees) * (rObj->GetY() + rObj->GetHeight() - y0) + x0;
	rotatedRect[3][1] = sin(degrees) * (rObj->GetX() - x0) + cos(degrees) * (rObj->GetY() + rObj->GetHeight() - y0) + y0;

	
	
	return true;
}

/*bool CCollisionDetector::SquareCircleCollides(CBaseObject* rect, ISolid* circle) //circle must have height == width
{
	int circleDistancex = abs(circle->GetX()+circle->GetWidth() - rect->GetX()+rect->GetWidth());
	int circleDistancey = abs(circle->GetY() + circle->GetWidth() - rect->GetY() + rect->GetHeight());

	if (circleDistancex > (rect->GetWidth() / 2 + circle->GetHeight())) { return false; }
	if (circleDistancey > (rect->GetHeight() / 2 + circle->GetWidth())) { return false; }

	if (circleDistancex <= (rect->GetWidth() / 2)) { return true; }
	if (circleDistancey <= (rect->GetHeight() / 2)) { return true; }

	int cornerDistance_sq = (circleDistancex - rect->GetWidth() / 2) ^ 2 +
		(circleDistancey - rect->GetHeight() / 2) ^ 2;

	return (cornerDistance_sq <= (circle->GetHeight() ^ 2));
}*/