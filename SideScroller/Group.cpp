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
	braveryBoost = 0;
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
			if (M(i)->tookDamage && groupIsAwareOfPlayer == false)
			{
				for (int k = 0; k < groupMembers.size(); k++)
				{
					groupMembers[k]->tookDamage == false;
				}
				GroupGainAwarenessOfPlayer();
				lastKnownDirection = Ai()->GetPlayerHorizontalDirection(M(i));

			}
			if (Ai()->Chance(50))
			{
				if (Ai(i)->SeesPlayer(900, M(i)))
				{
					M(i)->SetPlayerVisible(true);
					if (activity == GroupActivity::idle)
					{
						CEnemy * closest = Ai(0)->GetEnemyClosestToPlayerFromVector(groupMembers);
						closest->currentActivity = CEnemy::CurrentActivity::spottedPlayer;
					}
					if (!groupIsAwareOfPlayer || activity == GroupActivity::searching)
					{
						GroupGainAwarenessOfPlayer();
					}
					timeSincePlayerSpotted = 0;
				}
				else
				{
					M(i)->SetPlayerVisible(false);
				}
			}
		}
	}
}
bool CGroup::IsGroupBrave()
{
	int braveryPool = 0;
	for (int i = 0; i < groupMembers.size(); i++)
	{
		braveryPool += M(i)->bravery;
	}
	return (braveryPool + braveryBoost > lowerBraveryLimit);
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

CAI * CGroup::Ai()
{
	return AIVector->at(0);
}
bool CGroup::Update()
{
	if (Ai()->GroupDistanceToPlayer(groupMembers) > 1920) return false;
	if (groupMembers.size() == 0) return true;
	DecreaseDelays();
	ScanForPlayer();


	for (int i = 0; i < groupMembers.size(); i++)
	{
		if (M(i)->currentActivity == CEnemy::CurrentActivity::fighting
			|| M(i)->currentActivity == CEnemy::CurrentActivity::braveryboost
			|| M(i)->currentActivity == CEnemy::CurrentActivity::spottedPlayer
			|| M(i)->currentActivity == CEnemy::CurrentActivity::retreating)
		{
			if (M(i)->timeUntilAimCorrection >= 0)
			{
				if (M(i)->GetPlayerVisible())
					M(i)->SetOptimalXhairAngle(AIVector->at(M(i)->AI)->GetOptimalXhairAngle(M(i)));
			}
			else
			{
				if (M(i)->GetPlayerVisible())
					M(i)->timeUntilAimCorrection -= g_time;
			}
			M(i)->Aim();
		}
	}

	if (IsAlone())
	{
		if (Ai()->ContinuousChance(32000))
		{
			CEnemy::CurrentActivity tempActivity = M(0)->currentActivity;
			M(0)->currentActivity = CEnemy::CurrentActivity::alone;
			speechManager->AddSpeech(M(0), Ai()->GetSpeechString(M(0), IsCaptain(M(0))), GetColor(M(0)));
		}
	}

	if (groupIsAwareOfPlayer && timeSincePlayerSpotted > 7000 && activity != GroupActivity::alert)
	{
		groupIsAwareOfPlayer = false;
		for (int i = 0; i < groupMembers.size(); i++)
		{
			M(i)->SetAwareOfPlayer(false);
			M(i)->currentActivity = CEnemy::CurrentActivity::alert;
			M(i)->SetXhairAngle(0);
		}
		activity = GroupActivity::alert;
	}
	else if (groupIsAwareOfPlayer && timeSincePlayerSpotted > 2000 && activity != GroupActivity::searching)
	{
		activity = GroupActivity::searching;
		InitDelays();
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (Ai()->Chance(40))
			{
				M(i)->SetAwareOfPlayer(false);
			}
			if (M(i)->currentActivity != CEnemy::CurrentActivity::deserting)
				M(i)->currentActivity = CEnemy::CurrentActivity::searching;
		}
	}

	if (!losingMorals)
	{
		if (!IsGroupBrave() && AIVector->at(0)->ContinuousChance(2000))
		{
			LoseMorals();
		}
	}
	else
	{
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (M(i)->currentActivity == CEnemy::CurrentActivity::aboutToDesert)
			{
				if (Ai(i)->ContinuousChance(1600))
				{
					M(i)->currentActivity = CEnemy::CurrentActivity::deserting;
					if (Ai()->Chance(50))
					{
						speechManager->AddSpeech(M(i), Ai(i)->GetSpeechString(M(i), IsCaptain(M(i))), GetColor(i));
					}
				}
			}
		}
	}


	switch (activity)
	{
	case GroupActivity::alert:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			Ai(i)->Idle(M(i));
			if (Ai(i)->ContinuousChance(12000) && !IsAlone())
			{
				speechManager->AddSpeech(M(i), Ai(i)->GetSpeechString(M(i), IsCaptain(M(i))), GetColor(i));
			}
		}
		break;
	case GroupActivity::idle:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			Ai(i)->Idle(M(i));
			if (Ai(i)->ContinuousChance(22000))
			{
				speechManager->AddSpeech(M(i), Ai(i)->GetSpeechString(M(i), IsCaptain(M(i))), GetColor(i));
			}
		}
		break;
	case GroupActivity::gainingAwareness:
	{
											bool allAware = true;
											for (int i = 0; i < groupMembers.size(); i++)
											{
												if (!M(i)->IsAwareOfPlayer())
												{
													if (IsDelayZero(i))
													{
														M(i)->SetAwareOfPlayer(true);
														if (M(i)->currentActivity == CEnemy::CurrentActivity::spottedPlayer)
														{
															if (Ai()->Chance(90) && groupMembers.size() > 1)
															{
																speechManager->AddSpeech(M(i), AIVector->at(M(i)->AI)->GetSpeechString(M(i), (IsCaptain(M(i)))), GetColor(M(i)));
															}

														}
														else if (M(i) == captain && groupMembers.size() > 1)
														{
															if (Ai()->Chance(50))
															{
																captain->currentActivity = CEnemy::CurrentActivity::spottedPlayer;
																speechManager->AddSpeech(captain, AIVector->at(captain->AI)->GetSpeechString(captain, true), captainColor);
															}
														}
													}
													else allAware = false;
												}
												else
												{
													M(i)->currentActivity = CEnemy::CurrentActivity::fighting;
													Ai(i)->Attack(M(i));
													if (M(i)->GetPlayerVisible())
														M(i)->Aim();
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
				if (M(i)->currentActivity == CEnemy::CurrentActivity::retreating)
					Ai(i)->Retreat(M(i));
			}
			if (Ai()->ContinuousChance(7000))
			{
				InitDelays();
				activity = GroupActivity::fighting;

			}
		}
		break;

	case GroupActivity::fighting:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (IsDelayZero(i))
			{
				if (M(i)->currentActivity != CEnemy::CurrentActivity::deserting)
				{
					Ai(i)->Attack(M(i));
					if (Ai(i)->ContinuousChance(20000))
						speechManager->AddSpeech(M(i), Ai(i)->GetSpeechString(M(i), (IsCaptain(M(i)))), GetColor(M(i)));
				}

			}
		}
		if (Ai()->ContinuousChance(100000) && IsCaptainAlive() && HasSufferedCasualties() && groupMembers.size() > 1)
		{
			captain->currentActivity = CEnemy::CurrentActivity::retreating;
			speechManager->AddSpeech(captain, Ai(0)->GetSpeechString(captain, true), captainColor);
			activity = GroupActivity::retreating;
			InitDelays();
			for (int i = 0; i < groupMembers.size(); i++)
			{
				if (M(i)->currentActivity != CEnemy::CurrentActivity::deserting)
				{
					M(i)->currentActivity = CEnemy::CurrentActivity::retreating;
				}
			}
		}
		if (Ai()->ContinuousChance(30000) && IsCaptainAlive() && captain->currentActivity != CEnemy::CurrentActivity::deserting && losingMorals)
		{
			braveryBoost = 100;
			captain->currentActivity = CEnemy::CurrentActivity::braveryboost;
			if (!IsAlone())
				speechManager->AddSpeech(captain, Ai()->GetSpeechString(captain, true), GetColor(captain));
			captain->currentActivity = CEnemy::CurrentActivity::fighting;
			for (int i = 0; i < groupMembers.size(); i++)
			{
				if (groupMembers[i]->currentActivity == CEnemy::CurrentActivity::deserting)
				{
					groupMembers[i]->currentActivity = CEnemy::CurrentActivity::fighting;
				}
			}
		}
		lastKnownDirection = Ai()->GetPlayerHorizontalDirection(Ai()->GetEnemyClosestToPlayerFromVector(groupMembers));
	case GroupActivity::searching:
		for (int i = 0; i < groupMembers.size(); i++)
		{
			if (M(i)->currentActivity == CEnemy::CurrentActivity::searching)
			{
				Ai(i)->Search(M(i), lastKnownDirection);
			}
		}
		if (Ai(0)->ContinuousChance(15000) && !IsAlone())
		{
			CEnemy * closest = Ai()->GetEnemyClosestToPlayerFromVector(groupMembers);
			speechManager->AddSpeech(closest, Ai(closest)->GetSpeechString(closest, IsCaptain(closest)), GetColor(closest));
		}
	}
	return false;
}
bool CGroup::IsAlone()
{
	return (groupMembers.size() == 1);
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
	timeSincePlayerSpotted = 0;
}
void CGroup::LoseMorals()
{
	losingMorals = true;
	for (int i = 0; i < groupMembers.size(); i++)
	{
		if (M(i)->bravery < 10)
		{
			M(i)->currentActivity = CEnemy::CurrentActivity::aboutToDesert;
		}
		else
		{
			if (Ai()->Chance(50))
			{
				if (IsCaptain(M(i)))
				{
					if (Ai()->Chance(50))
					{
						M(i)->currentActivity = CEnemy::CurrentActivity::deserting;
						speechManager->AddSpeech(M(i), Ai(i)->GetSpeechString(M(i), true), captainColor);
					}
				}
				else
				{
					M(i)->currentActivity = CEnemy::CurrentActivity::aboutToDesert;
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
		if (M(i)->currentActivity == CEnemy::CurrentActivity::spottedPlayer)
			delayVector.push_back(minReactionTime);
		else
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
CEnemy* CGroup::M(int index)
{
	return groupMembers[index];
}