#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H
#include "PhysicsObject.h"
#include "BaseObject.h"

#include "Tile.h"
class CCollisionDetector
{
public:
	CCollisionDetector();
	virtual ~CCollisionDetector();
	bool Collides(CBaseObject * obj1, CBaseObject * obj2);
	bool Collides(CPhysicsObject * obj, const CTile * tile, int standardTileSize);
	bool Collides(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	//bool SquareCircleCollides(CBaseObject * square, ISolid * circle);
	bool RotatedCollides(CBaseObject * rotatedObj, CBaseObject * obj, int x0, int y0);
};

#endif