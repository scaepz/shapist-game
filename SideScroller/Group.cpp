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
			if (AIVector->at(0)->IsPlayerInLineOfSight(1000, groupMembers[i]))
			{
				groupMembers[i]->SetPlayerVisible(true);
				//text line here in the style of "there he is!"/"stop right there!"/"got him!" on enemy that made discovery
				if (!groupIsAwareOfPlayer)
				{
					GroupGainAwarenessOfPlayer();
				}
				std::cout << "Player spotted!" << std::endl;
				timeSincePlayerSpotted = 0;
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
	if (activityDelay > 0)
		activityDelay -= g_time;
	if (playerScanDelay > 0)
		playerScanDelay -= g_time;
	timeSincePlayerSpotted += g_time;
}
bool CGroup::IsDelayZero(int index)
{
	return (delayVector[index] <= 0);
}
bool CGroup::IsActivityTimerZero()
{
	return (activityDelay <= 0);
}
void CGroup::ResetActivityTimer()
{
	activityDelay = 100;
}
bool CGroup::Update()
{
	DecreaseDelays();
	ScanForPlayer();
	if (timeSincePlayerSpotted > 10000)
	{
		groupIsAwareOfPlayer = false;
		activity = GroupActivity::idle;
	}
	if (activity != GroupActivity::lostMorals)
	{
		if (!IsGroupBrave() && IsActivityTimerZero())
		{
			ResetActivityTimer();
			std::cout << "Group lost morals" << std::endl;
			LoseMorals();
		}
	}
	switch (activity)
	{
	case GroupActivity::idle:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			AIVector->at(groupMembers[i]->AI)->Idle(groupMembers[i]);
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
													}
													else allAware = false;
												}
											}
											if (allAware) {
												std::cout << "Everyone aware of player" << std::endl; activity = GroupActivity::fighting;
											};
	}
		break;
	case GroupActivity::retreating:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (IsDelayZero(i))
			{
				AIVector->at(groupMembers[i]->AI)->Retreat(groupMembers[i]);
				std::cout << "Group member retreated" << std::endl;
			}
		}
		break;
	case GroupActivity::lostMorals:
		if (IsActivityTimerZero() && IsCaptainAlive())

		{	//if rand true
			//Captain orders braveryboost, deserters might return
			std::cout << "Braveryboost" << std::endl;
			ResetActivityTimer();
		}
		break;
	case GroupActivity::fighting:
		if (IsActivityTimerZero() && IsCaptainAlive())
		{
			if (rand() % retreatRarity == 1)
				std::cout << "Retreat" << std::endl;
			ResetActivityTimer();
			//appropriate text here
			activity = GroupActivity::retreating;
		}
	}
	return (groupMembers.size() < 0);
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
			AIVector->at(groupMembers[i]->AI)->Retreat(groupMembers[i]);
		}
		else
		{
			if (rand() % 2 == 1)
			{
				if (IsCaptain(groupMembers[i]))
				{
					if (rand() % 3 == 1)
						AIVector->at(groupMembers[i]->AI)->Retreat(groupMembers[i]);
				}
				else
					AIVector->at(groupMembers[i]->AI)->Retreat(groupMembers[i]);
			}
		}
	}
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