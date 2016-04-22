#include "AI.h"
#include "SlowMoveCommand.h"
#include <iostream>
#include <fstream>
#include "FileReader.h"
#include <exception>
using std::cout;
using std::endl;
CAI::CAI(CVectorManager* _vm)
{
	vm = _vm;
	tileVector = vm->GetTileVector();
	player = vm->GetPlayer();
}

void CAI::Idle(CEnemy* self)
{
	switch (self->currentIdleActivity)
	{
	case CEnemy::CurrentIdleActivity::idle:
		if (ContinuousChance(3500))
		{
			int newActivity = rand() % (int)CEnemy::CurrentIdleActivity::numberOfIdleActivites;
			switch (newActivity)
			{
			case (int)CEnemy::CurrentIdleActivity::lookingLeft:
				self->ChangeAnimation(self->move);
				self->lookingLeft = true;
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingLeft;
				break;
			case (int)CEnemy::CurrentIdleActivity::lookingRight:
				self->ChangeAnimation(self->move);
				self->lookingLeft = false;
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingRight;
				break;
			case (int)CEnemy::CurrentIdleActivity::movingLeft:
				if (Chance(50))
				{
					SoftMoveLeft(self);
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::movingLeft;
					break;
				}
			case (int)CEnemy::CurrentIdleActivity::movingRight:
				if (Chance(50))
				{
					SoftMoveRight(self);
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::movingRight;
					break;
				}
			default:
				self->ChangeAnimation(self->idle_rare);
				break;
			}
		}
		break;
	case CEnemy::CurrentIdleActivity::movingLeft:
	case CEnemy::CurrentIdleActivity::movingRight:

		if (ContinuousChance(2400))
		{
			SoftStop(self);
			self->ChangeAnimation(self->idle);
			self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
		}
		break;

	case CEnemy::CurrentIdleActivity::lookingLeft:
		if (ContinuousChance(2500))
		{
			if (Chance(33))
			{
				self->ChangeAnimation(self->idle);
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
			}
			else
			{
				self->ChangeAnimation(self->move);
				self->lookingLeft = false;
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingRight;
				break;
			}
		}
		break;
	case CEnemy::CurrentIdleActivity::lookingRight:
		if (ContinuousChance(2500))
		{
			if (Chance(33))
			{
				self->ChangeAnimation(self->idle);
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
			}
			else
			{
				self->ChangeAnimation(self->move);
				self->lookingLeft = true;
				self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingLeft;
				break;
			}
		}
		break;
	}
}

void CAI::Attack(CEnemy* self)
{
	if (self->currentActivity != CEnemy::CurrentActivity::fighting)
	{
		self->currentActivity = CEnemy::CurrentActivity::fighting;
	}
	self->currentAnim = self->move;
	PlayerDirection pDir = GetPlayerHorizontalDirection(self);

	if (pDir == PlayerDirection::left)
	{
		if (ContinuousChance(2500))
			MoveLeft(self);
		else if (ContinuousChance(2500))
			SoftStop(self);
		else if (ContinuousChance(2500))
			SoftMoveRight(self);
		else if (ContinuousChance(4500))
			MoveRight(self);
		else if (ContinuousChance(3000))
			SoftMoveLeft(self);
	}
	else if (pDir == PlayerDirection::right)
	{
		if (ContinuousChance(2500))
			MoveRight(self);
		else if (ContinuousChance(2500))
			SoftStop(self);
		else if (ContinuousChance(2500))
			SoftMoveLeft(self);
		else if (ContinuousChance(4500))
			MoveLeft(self);
		else if (ContinuousChance(3000))
			SoftMoveRight(self);
	}
	else
	{
		SoftStop(self);
	}
	if (DistanceToPlayer(self) < 700)
	{
		if (self->GetPlayerVisible())
		{
			if (React(self))
				Fire(self);
		}
		else
		{
			CeaseFire(self);
		}
	}
	if (ContinuousChance(3000))
	{
		Jump(self);
	}
}
void CAI::Desert(CEnemy* self)
{
	std::cout << "Desert";
	if (self->currentActivity != CEnemy::CurrentActivity::deserting)
	{
		self->currentActivity = CEnemy::CurrentActivity::deserting;
	}
	PlayerDirection pDir = GetPlayerHorizontalDirection(self);
	if (pDir == PlayerDirection::right)
	{
		if (React(self))
			MoveLeft(self);
	}
	else
	{
		if (React(self))
			MoveRight(self);
	}
	if (ContinuousChance(3000))
	{
		Jump(self);
	}
}
void CAI::Search(CEnemy* self, PlayerDirection lastKnownDirection)
{
	if (self->currentActivity != CEnemy::CurrentActivity::searching)
	{
		self->currentActivity = CEnemy::CurrentActivity::searching;
	}
	if (lastKnownDirection == PlayerDirection::right)
	{
		MoveRight(self);
	}
	else if (lastKnownDirection == PlayerDirection::left)
	{
		MoveLeft(self);
	}
	if (ContinuousChance(1500))
	{
		Jump(self);
	}
}


void CAI::Retreat(CEnemy* self)
{

	PlayerDirection pDir = GetPlayerHorizontalDirection(self);

	if (pDir == PlayerDirection::right)
	{
		if (React(self))
			MoveLeft(self);
	}
	else
	{
		if (React(self))
			MoveRight(self);
	}
	if (ContinuousChance(3000))
	{
		Jump(self);
	}
	if (self->GetPlayerVisible())
	{
		self->Aim();
		Fire(self);
	}
}
void CAI::MoveLeft(CEnemy* self)
{
	CSoftStopCommand * s = new CSoftStopCommand(self);
	vm->DeleteObject(s);
	delete s;
	CMoveCommand * m = new CMoveCommand(self, 1);
	vm->DeleteObject(m);
	delete m;
	CMoveCommand * move = new CMoveCommand(self, 0);
	vm->AddObject(move);

	self->ChangeAnimation(self->move);
	self->lookingLeft = true;
}
void CAI::Jump(CEnemy* self)
{
	CJumpCommand * j = new CJumpCommand(self);
	vm->AddObject(j);
}
void CAI::MoveRight(CEnemy* self)
{
	CSoftStopCommand * s = new CSoftStopCommand(self);
	vm->DeleteObject(s);
	delete s;

	CMoveCommand * m = new CMoveCommand(self, 0);
	vm->DeleteObject(m);
	delete m;
	CMoveCommand * move = new CMoveCommand(self, 1);
	vm->AddObject(move);

	self->ChangeAnimation(self->move);
	self->lookingLeft = false;
}
bool CAI::React(CEnemy* self)
{
	if (self->IsReacting())
	{
		return self->HasReacted();
	}
	else
	{
		self->React();
		return false;
	}
}
void CAI::Fire(CEnemy* self)
{
	CAttackCommand * a = new CAttackCommand(self);
	vm->AddObject(a);
}
void CAI::CeaseFire(CEnemy* self)
{
	CAttackCommand * a = new CAttackCommand(self);
	vm->DeleteObject(a);
	delete a;
}
void CAI::Stop(CEnemy* self)
{
	CMoveCommand * m = new CMoveCommand(self, 0);
	vm->DeleteObject(m);
	delete m;

	m = new CMoveCommand(self, 1);
	vm->DeleteObject(m);
	delete m;

	CStopCommand * s = new CStopCommand(self);
	vm->AddObject(s);
}
void CAI::SoftStop(CEnemy* self)
{
	CMoveCommand * r = new CMoveCommand(self, 0);
	vm->DeleteObject(r);
	delete r;
	r = new CMoveCommand(self, 1);
	vm->DeleteObject(r);
	delete r;

	CSoftStopCommand * s = new CSoftStopCommand(self);
	vm->AddObject(s);
}
void CAI::SoftMoveRight(CEnemy* self)
{
	CSoftStopCommand * s = new CSoftStopCommand(self);
	vm->DeleteObject(s);
	delete s;

	CCommandObject * cmd = new CSlowMoveCommand(self, 1);
	self->ChangeAnimation(self->move);
	self->lookingLeft = false;
	vm->AddObject(cmd);
}
void CAI::SoftMoveLeft(CEnemy* self)
{
	CSoftStopCommand * s = new CSoftStopCommand(self);
	vm->DeleteObject(s);
	delete s;

	CCommandObject * cmd = new CSlowMoveCommand(self, 0);
	self->ChangeAnimation(self->move);
	self->lookingLeft = true;
	vm->AddObject(cmd);
}
CAI::PlayerDirection CAI::GetPlayerHorizontalDirection(CAnimate* self)
{
	if (player->GetX() > self->GetX() + self->GetWidth())
		return CAI::PlayerDirection::right;
	else if (player->GetX() + player->GetWidth() < self->GetX())
		return CAI::PlayerDirection::left;
	else return CAI::PlayerDirection::center;
}
CAI::PlayerDirection CAI::GetPlayerVerticalDirection(CAnimate* self)
{
	if (player->GetY() > self->GetY() + self->GetHeight())
		return CAI::PlayerDirection::below;
	else if (player->GetY() + player->GetHeight() < self->GetY())
		return CAI::PlayerDirection::above;
	else return CAI::PlayerDirection::center;
}
int CAI::QuickDistanceToPlayer(CEnemy * self)
{
	return sqrt((player->GetX() - self->GetX()) * (player->GetX() - self->GetX()) + (player->GetY() - self->GetY()) * (player->GetY() - self->GetY()));
}
int CAI::DistanceToPlayer(CEnemy * self)
{
	int horDist = player->GetX() - self->GetX();
	CAI::PlayerDirection dir = GetPlayerHorizontalDirection(self);
	if (dir == CAI::PlayerDirection::left)
		horDist -= player->GetWidth();
	else if (dir == CAI::PlayerDirection::right)
		horDist -= self->GetWidth();
	else if (dir == CAI::PlayerDirection::center)
		horDist = 0;

	int verDist = player->GetY() - self->GetY();
	dir = GetPlayerVerticalDirection(self);
	if (dir == CAI::PlayerDirection::above)
		verDist -= player->GetHeight();
	else if (dir == CAI::PlayerDirection::below)
		verDist -= self->GetHeight();
	else if (dir == CAI::PlayerDirection::center)
		verDist = 0;

	return sqrt(horDist * horDist + verDist * verDist);


}
bool CAI::ContinuousChance(int averageMsBetweenOccurence)
{ 
	return (rand() % (averageMsBetweenOccurence / g_time) == 0);
}
bool CAI::Chance(int percent)
{
	if (percent == 0) return false;
	return ((rand() % 100) < percent);
}
std::string CAI::GetSpeechString(CEnemy * self, bool captain)
{
	pugi::xml_document doc;
	doc.load_file(vm->GetSpeechPath());

	string activity = self->GetActivityString();

	string role;
	if (captain) role = "captain"; else role = "common";

	for (pugi::xml_node node = doc.child("speech").first_child(); node; node = node.next_sibling())
	{
		string a = node.name();
		if (a == role)
		for (pugi::xml_node rolenode = node.first_child(); rolenode; rolenode = rolenode.next_sibling())
		{
			a = rolenode.name();
			if (a == activity)
			{
				int amount = 0;
				pugi::xml_node linenode;
				vector<string> lineVector;
				for (pugi::xml_node linenode = rolenode.first_child(); linenode; linenode = linenode.next_sibling())
				{
					lineVector.push_back(linenode.text().as_string());
				}
				if (lineVector.size() == 0) return "";
				string line = lineVector.at(rand() % lineVector.size()); 
				std::cout << line << std::endl;
				return line;
			}
		}
	}
	
	return "";
}
CAI::~CAI()
{
}
float CAI::GetOptimalXhairAngle(CEnemy* self)
{
	return atan2(vm->GetPlayer()->GetY() - self->GetY(), vm->GetPlayer()->GetX() - self->GetX());
}
CEnemy * CAI::GetEnemyClosestToPlayerFromVector(vector<CEnemy*> group)
{
	if (group.size() == 0) return nullptr;
	CEnemy * closest = group[0];
	int lowestDistance = 80000;
	for (int i = 0; i < group.size(); i++)
	{
		int tempDistance = QuickDistanceToPlayer(group[i]);
		if (tempDistance < lowestDistance)
		{
			closest = group[i];
			lowestDistance = tempDistance;
		}
	}
	return closest;
}

int CAI::GroupDistanceToPlayer(vector<CEnemy*> group)
{
	if (group.size() == 0) return -1;
	int lowestDistance = 80000;
	for (int i = 0; i < group.size(); i++)
	{
		int tempDistance = QuickDistanceToPlayer(group[i]);
		if (tempDistance < lowestDistance)
		{
			lowestDistance = tempDistance;
		}
	}
	return lowestDistance;
}

bool CAI::SeesPlayer(int maxDistance, CEnemy * self)
{
	if (QuickDistanceToPlayer(self) > maxDistance) return false;
	int tileSize = vm->standardTileSize;

	int x1 = self->GetX() + self->GetWidth() / 2;
	int y1 = self->GetY() + self->GetHeight() / 4;

	int x2;
	int y2;

	if (vm->GetTileVector()->at(y1 / vm->standardTileSize).at(x1 / vm->standardTileSize) != nullptr)
	{
		return false;
	}

	vector<vector<CTile*>>* tileVector = vm->GetTileVector();
	for (int i = 0; i < 2; i++)
	{

		bool seesPlayer = true;
		switch (i)
		{
		case 0:
			x2 = player->GetX();
			y2 = player->GetY() + player->GetHeight()/2;
			break;
		case 1:
			x2 = player->GetX() + player->GetWidth();
			y2 = player->GetY() + player->GetHeight() / 2;
			break;
		}
		int xAddition = 1;
		int yAddition = 1;

		int xBorder = 0;
		int yBorder = 0;
		if (x2 < x1)
		{
			xAddition = -1;
			xBorder = tileSize - 1;
		}
		if (y2 < y1)
		{
			yAddition = -1;
			yBorder = tileSize - 1;
		}
		float m = (float)((float)(y2 - y1) / (float)(x2 - x1)); //riktningskoefficient / slope
		float yIterator = y1;
		if (abs(m) < 1.5f)
		{
			for (int xIterator = x1; xIterator != x2; xIterator += xAddition)
			{
				yIterator += m * xAddition; //when x increases with 1, y increases with m
				if (xIterator / tileSize >= 0 && xIterator / tileSize < vm->GetTileVector()->at(0).size() && yIterator / tileSize >= 0 && yIterator / tileSize < vm->GetTileVector()->size()) //outside of map check
				{
					if (tileVector->at(yIterator / tileSize).at(xIterator / tileSize) != nullptr)
					{
						seesPlayer = false;
						break;
					}
				}
			}
		}
		else
		{
			float xIterator = x1;
			m = (float)((float)(x2 - x1) / (float)(y2 - y1));
			for (int yIterator = y1; yIterator != y2; yIterator += yAddition)
			{
				xIterator += m * yAddition; //when y increases with 1, x increases with m
				if (xIterator / tileSize >= 0 && xIterator / tileSize < vm->GetTileVector()->at(0).size() && yIterator / tileSize >= 0 && yIterator / tileSize < vm->GetTileVector()->size())//outside of map check
				{
					if (tileVector->at(yIterator / tileSize).at(xIterator / tileSize) != nullptr)
					{
						seesPlayer = false;
						break;
					}
				}
			}
		}
		if (seesPlayer) return true;
	}
	return false;
}