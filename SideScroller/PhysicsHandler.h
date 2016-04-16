#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H
#include <vector>
#include "Projectile.h"
#include "Global.h"
#include "Tile.h"
#include "Player.h"
#include "CollisionDetector.h"
#include "VectorManager.h"
using std::vector;
class CPhysicsHandler
{
public:
	CPhysicsHandler(CVectorManager *vm);
	~CPhysicsHandler();
	
	void DoPhysics();
private:
	CVectorManager * vm;
	int standardTileSize;
	vector<CPhysicsObject*> * objectVector;
	vector<vector<CTile*>> * tileVector;
	vector<CBaseObject*> * solidVector;
	CPlayer * player; //included for bandaid reasons
	double gravity;
	void FixCollision(int index);
	void ApplyGravity();
	void UpdateObjectPositions();
	float friction;

};

#endif