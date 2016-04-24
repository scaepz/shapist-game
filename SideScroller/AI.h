#ifndef AI_H
#define AI_H
#include <vector>
#include "Commands.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "Global.h"
#include "VectorManager.h"
#include "SDL.h"
#include "pugixml.hpp"
#include <string>
#include <map>
#include "SpeechManager.h"
using namespace pugi;
using std::vector;
class CAI
{
public:
	CAI(CVectorManager * _vm);
	virtual ~CAI();
	enum class PlayerDirection
	{
		above,
		below,
		left,
		right,
		center
	};
	virtual void Retreat(CEnemy* self);
	virtual void Attack(CEnemy* self);
	virtual void Idle(CEnemy* self);
	virtual void Desert(CEnemy* self);
	virtual void Search(CEnemy* self, PlayerDirection lastKnownDirection);
	void MoveRight(CEnemy* self);
	void MoveLeft(CEnemy* self);
	void Jump(CEnemy* self); 
	bool React(CEnemy* self);
	void Fire(CEnemy* self);
	void CeaseFire(CEnemy* self);
	void Stop(CEnemy* self);
	void SoftStop(CEnemy* self);
	void SoftMoveRight(CEnemy* self);
	void SoftMoveLeft(CEnemy* self);
	virtual std::string GetSpeechString(CEnemy * self, bool captain);
	float GetOptimalXhairAngle(CEnemy* self);
	int DistanceToPlayer(CEnemy* self);
	int QuickDistanceToPlayer(CEnemy * self);
	bool ContinuousChance(int averageMsBetweenOccurence);
	bool Chance(int percent);
	int GroupDistanceToPlayer(vector<CEnemy*> group);
	CEnemy * GetEnemyClosestToPlayerFromVector(vector<CEnemy*> group);
	int GetHorizontalDistanceToPlayer(CEnemy * self);
	PlayerDirection GetPlayerHorizontalDirection(CAnimate* self);
	PlayerDirection GetPlayerVerticalDirection(CAnimate* self);
//	bool IsPlayerInLineOfSight(int maxDistance, CEnemy * self);//expensive operation, only call once in a while
	bool SeesPlayer(int maxDistance, CEnemy* self);

	/* GetEnemyCrosshairPlacement
	do not call every frame, call only based on time
	x,y: give the previous x and y direction variables, get the new ones
	xvel yvel: give the previous x and y velocity variables, get the new ones (just for storing them for this function)
	optimalx, optimaly: enemypos - playerpos (perfect direction variables)
	*/
	void GetEnemyCrosshairPlacement(float * previousRotation, float *previousRotationVelocity, int optimalRotation);
protected:
	CPlayer * player;
	vector<vector<CTile*>>* tileVector;
	CVectorManager *vm;

};

#endif