#ifndef ENEMY_H
#define ENEMY_H
#include "Animate.h"


class CEnemy :
	public CAnimate
{
public:
	enum class CurrentActivity
	{
		idling,
		retreating,
		deserting,
		fighting,
		alert
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
	float GetXhairAngleDeg();
	CurrentIdleActivity currentIdleActivity;
	CurrentActivity currentActivity;
	CEnemy(int x, int y, int textureId, int size, vector<CWeapon*> & weaponVector);
	virtual ~CEnemy();
	//int GetFrame();
	void OnCollision(bool, bool);
	void SetRotation(float _rotation);
	//ai stuff
	int reactionTime;
	int timeUntilReaction;
	int rangeOfVision;
	int AITimer;
	int AI = 0;
	int aiParam[4];
	int bravery;
	
	bool GetPlayerVisible();
	bool GetPlayerVisibleStale();
	void SetPlayerVisible(bool);
	void UpdateTime(int time);
	void ResetTimeUntilReaction();
	void UpdateTimeUntilReaction(int time);
	int GetTimeUntilReaction();
	bool IsAwareOfPlayer(); 
	void SetAwareOfPlayer(bool);
	float crosshairVelocity;
private:
	bool awareOfPlayer;
	int timeSincePlayerNotVisible;
	bool playerVisible;
	int timeSincePlayerVisibilityCheck;

};

#endif