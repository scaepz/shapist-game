#include "Group.h"
#include <iostream>

CGroup::CGroup()
{
	captain = nullptr;
	playerScanDelay = playerScanDelayMax + rand() % 50;
	activity = GroupActivity::idle;
	groupIsAwareOfPlayer = false;
	captainColor.r = 255;
	captainColor.g = 10;
	captainColor.b = 0;
	captainColor.a = 255;
	memberColor.r = 240;
	memberColor.g = 230;
	memberColor.b = 140;
	memberColor.a = 255;
}
void CGroup::Init()
{
	lowerBraveryLimit = groupMembers.size() * 10;
	originalGroupSize = groupMembers.size();
	activity = GroupActivity::idle;
}
void CGroup::ScanForPlayer()
{
	if (playerScanDelay <= 0)
	{
		playerScanDelay = playerScanDelayMax;
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (AIVector->at(groupMembers[i]->AI)->IsPlayerInLineOfSight(1000, groupMembers[i]))
			{
				groupMembers[i]->SetPlayerVisible(true);
				if (activity == GroupActivity::idle)
				{
					CEnemy * closest = Ai(0)->GetEnemyClosestToPlayerFromVector(groupMembers);
					closest->currentActivity = CEnemy::CurrentActivity::spottedPlayer;
					if (rand() % 3 < 2)
					{
						speechManager->AddSpeech(closest, AIVector->at(groupMembers[i]->AI)->GetSpeechString(closest, (IsCaptain(closest))), GetColor(closest));
					}
				}
				if (!groupIsAwareOfPlayer || activity == GroupActivity::searching)
				{
					GroupGainAwarenessOfPlayer();
				}
				timeSincePlayerSpotted = 0;
			}
			else
			{
				groupMembers[i]->SetPlayerVisible(false);
			}
		}
	}
}
bool CGroup::IsGroupBrave()
{
	int braveryPool = 0;
	for (int i = 0; i < groupMembers.size(); i++)
	{
		braveryPool += groupMembers[i]->bravery;
	}
	return (braveryPool > lowerBraveryLimit);
}
bool CGroup::HasSufferedCasualties()
{
	return (groupMembers.size() < originalGroupSize);
}

bool CGroup::IsCaptainAlive()
{
	return (captain != nullptr);
}

void CGroup::OrderRetreat()
{
	activity = GroupActivity::retreating;
}//if captain is alive the group might retreat as a unit
void CGroup::DecreaseDelays()
{
	for (int i = 0; i < delayVector.size(); i++)
	{
		if (delayVector[i] > 0)
			delayVector[i] -= g_time;
	}
	if (playerScanDelay > 0)
		playerScanDelay -= g_time;
	timeSincePlayerSpotted += g_time;
}
bool CGroup::IsDelayZero(int index)
{
	return (delayVector[index] <= 0);
}
bool CGroup::Update()
{
	if (groupMembers.size() == 0) return false;
	DecreaseDelays();
	ScanForPlayer();

	for (int i = 0; i < groupMembers.size(); i++)
	{
		if (groupMembers[i]->currentActivity == CEnemy::CurrentActivity::fighting
			|| groupMembers[i]->currentActivity == CEnemy::CurrentActivity::braveryboost
			|| groupMembers[i]->currentActivity == CEnemy::CurrentActivity::spottedPlayer)
		{
			if (groupMembers[i]->timeUntilAimCorrection >= 0)
			{
				if (groupMembers[i]->GetPlayerVisible())
					groupMembers[i]->SetOptimalXhairAngle(AIVector->at(groupMembers[i]->AI)->GetOptimalXhairAngle(groupMembers[i]));
			}
			else
			{
				if (groupMembers[i]->GetPlayerVisible())
					groupMembers[i]->timeUntilAimCorrection -= g_time;
			}
			groupMembers[i]->Aim();
		}
	}

	if (groupIsAwareOfPlayer && timeSincePlayerSpotted > 7000)
	{
		groupIsAwareOfPlayer = false;
		for (int i = 0; i < groupMembers.size(); i++)
		{
			groupMembers[i]->SetAwareOfPlayer(false);
			groupMembers[i]->currentActivity = CEnemy::CurrentActivity::alert;
			groupMembers[i]->SetXhairAngle(0);
		}
		activity = GroupActivity::alert;
	}
	else if (groupIsAwareOfPlayer && timeSincePlayerSpotted > 2000)
	{
		activity = GroupActivity::searching;
		for (int i = 0; i < groupMembers.size(); i++)
		{
			groupMembers[i]->SetXhairAngle(0);
			if (groupMembers[i]->currentActivity != CEnemy::CurrentActivity::deserting)
				groupMembers[i]->currentActivity = CEnemy::CurrentActivity::searching;
		}
	}

	if (activity != GroupActivity::lostMorals)
	{
		if (!IsGroupBrave() && AIVector->at(0)->GetRandomBool(2000))
		{
			LoseMorals();
		}
	}


	switch (activity)
	{
	case GroupActivity::alert:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			Ai(i)->Idle(groupMembers[i]);
			if (Ai(i)->GetRandomBool(10000))
			{
				speechManager->AddSpeech(groupMembers[i], Ai(i)->GetSpeechString(groupMembers[i], IsCaptain(groupMembers[i])), GetColor(i));
			}
		}
		break;
	case GroupActivity::idle:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			Ai(i)->Idle(groupMembers[i]);
			if (Ai(i)->GetRandomBool(10000))
			{
				speechManager->AddSpeech(groupMembers[i], Ai(i)->GetSpeechString(groupMembers[i], IsCaptain(groupMembers[i])), GetColor(i));
			}
		}
		break;
	case GroupActivity::gainingAwareness:
	{
		bool allAware = true;
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (!groupMembers[i]->IsAwareOfPlayer())
			{
				if (IsDelayZero(i))
				{
					groupMembers[i]->SetAwareOfPlayer(true);
					if (groupMembers[i] == captain)
					{
						captain->currentActivity = CEnemy::CurrentActivity::spottedPlayer;
						speechManager->AddSpeech(captain, AIVector->at(captain->AI)->GetSpeechString(captain, true), captainColor);
					}
				}
				else allAware = false;
			}
			else
			{
				groupMembers[i]->currentActivity = CEnemy::CurrentActivity::fighting;
				Ai(i)->Attack(groupMembers[i]);
				if (groupMembers[i]->GetPlayerVisible())
					groupMembers[i]->Aim();
			}
		}
		if (allAware)
		{
			activity = GroupActivity::fighting;
		}

	}
	break;
	case GroupActivity::retreating:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (IsDelayZero(i))
			{
				Ai(i)->Retreat(groupMembers[i]);
			}
		}
		break;
	case GroupActivity::lostMorals:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (groupMembers[i]->currentActivity == CEnemy::CurrentActivity::aboutToDesert)
			{
				if (Ai(i)->GetRandomBool(1200))
				{
					groupMembers[i]->currentActivity = CEnemy::CurrentActivity::deserting;
					if (rand() % 3 > 0)
					{
						speechManager->AddSpeech(groupMembers[i], Ai(i)->GetSpeechString(groupMembers[i], false), memberColor);
					}
				}
			}
			if (groupMembers[i]->currentActivity == CEnemy::CurrentActivity::deserting)
			{
				Ai(i)->Desert(groupMembers[i]);
			}
		}
		if (AIVector->at(0)->GetRandomBool(20000) && IsCaptainAlive())
		{	//if rand true
			//Captain orders braveryboost, deserters might return
			std::cout << "Braveryboost" << std::endl;
		}
		break;
	case GroupActivity::fighting:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			AIVector->at(groupMembers[i]->AI)->Attack(groupMembers[i]);
		}
		if (AIVector->at(0)->GetRandomBool(40000) && IsCaptainAlive() && HasSufferedCasualties())
		{
			captain->currentActivity = CEnemy::CurrentActivity::retreating;
			speechManager->AddSpeech(captain, Ai(0)->GetSpeechString(captain, true), captainColor);
			activity = GroupActivity::retreating;
		}
		lastKnownDirection = Ai(0)->GetPlayerHorizontalDirection(groupMembers[0]);
	case GroupActivity::searching:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (groupMembers[i]->currentActivity == CEnemy::CurrentActivity::searching)
			{
				Ai(i)->Search(groupMembers[i], lastKnownDirection);
				if (Ai(i)->GetRandomBool(25000))
					speechManager->AddSpeech(groupMembers[i], Ai(i)->GetSpeechString(groupMembers[i], IsCaptain(groupMembers[i])), GetColor(i));
			}
		}
	}
	return false;
}

SDL_Color CGroup::GetColor(int index)
{
	if (IsCaptain(groupMembers[index])) return captainColor;
	else return memberColor;
}

void CGroup::GroupGainAwarenessOfPlayer()
{
	activity = GroupActivity::gainingAwareness;
	groupIsAwareOfPlayer = true;
	InitDelays();
}
void CGroup::LoseMorals()//if bravery is low (captain may be alive) units might desert
{
	for (int i = 0; i < groupMembers.size(); i++)
	{
		if (groupMembers[i]->bravery < 10)
		{
			groupMembers[i]->currentActivity = CEnemy::CurrentActivity::aboutToDesert;
		}
		else
		{
			if (rand() % 2 == 1)
			{
				if (IsCaptain(groupMembers[i]))
				{
					if (rand() % 2 == 1)
					{
						groupMembers[i]->currentActivity = CEnemy::CurrentActivity::deserting;
						speechManager->AddSpeech(groupMembers[i], Ai(i)->GetSpeechString(groupMembers[i], true), captainColor);
					}
				}
				else
				{
					groupMembers[i]->currentActivity = CEnemy::CurrentActivity::aboutToDesert;
				}
			}
		}
	}
}
CAI * CGroup::Ai(int memberIndex)
{
	return AIVector->at(groupMembers[memberIndex]->AI);
}
CGroup::~CGroup()
{
}
bool CGroup::IsCaptain(CAnimate* a)
{
	return (a == captain);
}
void CGroup::InitDelays()
{
	delayVector.clear();
	for (int i = 0; i < groupMembers.size(); i++)
	{
		delayVector.push_back((rand() % maxReactionTime) + minReactionTime);
	}
}
void CGroup::AddMember(CEnemy* newMember, bool isCaptain)
{
	groupMembers.push_back(newMember);
	if (isCaptain)
	{
		captain = newMember;
	}
}
bool CGroup::RemoveMember(CAnimate* memberToRemove)
{
	if (IsCaptain(memberToRemove)) captain = nullptr;
	for (int i = 0; i < groupMembers.size(); i++)
	{
		if (groupMembers.at(i) == memberToRemove)
		{
			groupMembers.erase(groupMembers.begin() + i);
			return true;
		}
	}
	return false;
}

SDL_Color CGroup::GetColor(CEnemy * member)
{
	if (IsCaptain(member)) return captainColor;
	else return memberColor;
}

CAI * CGroup::Ai(CEnemy * member)
{
	return AIVector->at(member->AI);
}