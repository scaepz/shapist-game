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
		searching,
		alert,
		numberOfActivites
	};
	CSpeechManager * speechManager;
private:
	SDL_Color captainColor;
	SDL_Color memberColor;
	SDL_Color GetColor(int index);
	SDL_Color GetColor(CEnemy * member);
	bool IsCaptainAlive();
	static const int maxReactionTime = 600; //actually max + min is real max
	static const int minReactionTime = 200;
	static const int playerScanDelayMax = 40;
	CEnemy* M(int index);
	int timeSincePlayerSpotted = 0;
	CAI * Ai(int memberIndex);
	CAI * Ai(CEnemy * member);
	CAI * Ai();
	int playerScanDelay;
	int originalGroupSize;
	bool groupIsAwareOfPlayer = false;
	void DecreaseDelays();
	bool HasSufferedCasualties();
	bool IsDelayZero(int index);
	CGroup::GroupActivity activity;
	bool IsCaptain(CAnimate*);
	bool IsGroupBrave();
	int lowerBraveryLimit;
	const int randomTalkDelay = 1500; //maybe not here
	void InitDelays();
	CAI::PlayerDirection lastKnownDirection;
	CEnemy * captain;
	std::vector<CEnemy*> groupMembers;
	std::vector<int> delayVector;
	int braveryBoost;

};

#endif