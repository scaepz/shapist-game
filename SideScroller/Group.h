#ifndef GROUP_H
#define GROUP_H
#include "Enemy.h"
#include "SpeechManager.h"
#include "AI.h"

class CGroup
{
public:
	CGroup();
	virtual ~CGroup();
	void Init();
	void OrderRetreat(); //if captain is alive the group might retreat as a unit
	void ScanForPlayer();
	bool Update();
	void GroupGainAwarenessOfPlayer();
	void LoseMorals(); //if bravery is low (captain may be alive) units might desert
	void AddMember(CEnemy*, bool isCaptain);
	bool RemoveMember(CAnimate*);
	std::vector<CAI*>* AIVector;
	enum class GroupActivity
	{
		idle,
		gainingAwareness,
		lostMorals,
		retreating,
		fighting,
		numberOfActivites
	};
	CSpeechManager * speechManager;
private:
	SDL_Color captainColor;
	SDL_Color memberColor;

	bool IsCaptainAlive();
	static const int maxReactionTime = 600; //actually max + min is real max
	static const int minReactionTime = 200;
	static const int playerScanDelayMax = 155;
	static const int retreatRarity = 200;
	static const int returnRarity = 50;
	static const int timeToForget = 10000;
	int timeSincePlayerSpotted = 0;

	int playerScanDelay;
	int activityDelay;
	bool IsActivityTimerZero();
	void ResetActivityTimer();
	int originalGroupSize;
	bool groupIsAwareOfPlayer = false;
	void DecreaseDelays();
	bool IsDelayZero(int index);
	CGroup::GroupActivity activity;
	bool IsCaptain(CAnimate*);
	bool IsGroupBrave();
	int lowerBraveryLimit;
	const int randomTalkDelay = 1500; //maybe not here
	void InitDelays();

	CEnemy * captain;

	std::vector<CEnemy*> groupMembers;
	std::vector<int> delayVector;

};

#endif