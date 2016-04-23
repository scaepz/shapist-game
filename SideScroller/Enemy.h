#ifndef ENEMY_H
#define ENEMY_H
#include "Animate.h"
#include <string>
#include "pugixml.hpp"

class CEnemy :
	public CAnimate
{
public:
	enum class CurrentActivity
	{
		idling,
		retreating,
		aboutToDesert,
		deserting,
		fighting,
		alert,
		braveryboost,
		spottedPlayer,
		searching,
		alone
	};
	enum class CurrentIdleActivity
	{
		idle,
		lookingLeft,
		lookingRight,
		movingLeft,
		movingRight,
		numberOfIdleActivites
	};
	pugi::char_t * speechType;
	pugi::char_t * GetActivityString();	
	CurrentIdleActivity currentIdleActivity;
	CurrentActivity currentActivity;
	virtual void TakeDamage(int damage);
	CEnemy(int x, int y, int textureId, int size, vector<CWeapon*> & weaponVector);
	virtual ~CEnemy();
	void OnCollision(bool, bool);
	void SetRotation(float _rotation);
	void UpdateTime(int time);

	
private:

	
	///////////AI
	//Reaction
public:
	void React();
	bool IsReacting();
	bool HasReacted();
private:
	int timeUntilReaction;
	const int REACTION_TIME = 330;
	bool isReacting;

	//Xhair
public:	
	void Aim();
	float GetXhairAngleDeg();
	void SetOptimalXhairAngle(float angle);
	int timeUntilAimCorrection;
private:
	float xhairAngleRandomAddition;
	float xhairVelocity;
	float optimalAngle;	

	//Player Awareness
public:
	bool IsAwareOfPlayer();
	void SetAwareOfPlayer(bool);
	bool GetPlayerVisible();
	bool GetPlayerVisibleStale();
	void SetPlayerVisible(bool);
	bool tookDamage;
private:
	bool awareOfPlayer;
	int timeSincePlayerNotVisible;
	bool playerVisible;
	int timeSincePlayerVisibilityCheck;
	//Misc
public:
	int AI = 0;
	int aiParam[4];
	int bravery;
	/////////////
};

#endif