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
	if (self->currentActivity != CEnemy::CurrentActivity::idling)
	{
		self->currentActivity = CEnemy::CurrentActivity::idling;
	}
	GetSpeechString(self, true);
	switch (self->currentIdleActivity)
	{
	case CEnemy::CurrentIdleActivity::idle:
		if (GetRandomBool(3500))
		{
			int newActivity = rand() % (int)CEnemy::CurrentIdleActivity::numberOfIdleActivites;
			cout << newActivity << endl;
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

		if (GetRandomBool(2400))
		{
			SoftStop(self);
			self->ChangeAnimation(self->idle);
			self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
		}
		break;

	case CEnemy::CurrentIdleActivity::lookingLeft:
		if (GetRandomBool(2500))
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
		if (GetRandomBool(2500))
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
		if (GetRandomBool(2500))
			MoveLeft(self);
		else if (GetRandomBool(2500))
			SoftStop(self);
		else if (GetRandomBool(2500))
			SoftMoveRight(self);
		else if (GetRandomBool(4500))
			MoveRight(self);
		else if (GetRandomBool(3000))
			SoftMoveLeft(self);
	}
	else if (pDir == PlayerDirection::right)
	{
		if (GetRandomBool(2500))
			MoveRight(self);
		else if (GetRandomBool(2500))
			SoftStop(self);
		else if (GetRandomBool(2500))
			SoftMoveLeft(self);
		else if (GetRandomBool(4500))
			MoveLeft(self);
		else if (GetRandomBool(3000))
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
	if (GetRandomBool(3000))
	{
		Jump(self);
	}
}
void CAI::Desert(CEnemy* self)
{
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
	if (GetRandomBool(3000))
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
	if (GetRandomBool(1500))
	{
		Jump(self);
	}
}


void CAI::Retreat(CEnemy* self)
{
	if (self->currentActivity != CEnemy::CurrentActivity::retreating)
	{
		self->currentActivity = CEnemy::CurrentActivity::retreating;
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
	if (GetRandomBool(3000))
	{
		Jump(self);
	}
	if (self->GetPlayerVisible())
	{
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
bool CAI::GetRandomBool(int averageMsBetweenOccurence)
{
	return (rand() % averageMsBetweenOccurence / g_time == 0);
}
bool CAI::Chance(int percent)
{
	if (percent == 0) return false;
	return (rand() % 100 / percent);
}
std::string CAI::GetSpeechString(CEnemy * self, bool captain)
{
	pugi::xml_document doc;

	doc.load_file(vm->GetSpeechPath());

	char_t * activity = self->GetActivityString();
	vector<string> possibleLines;

	char_t * role;
	if (captain) role = "captain"; else role = "common";

	char_t * index = new char_t[2];
	string test = doc.child("speech").child(self->speechType).child(role).child(activity).child("amount").text().as_string();
	int numberOfChildren = doc.child("speech").child(self->speechType).child(role).child(activity).child("amount").text().as_int();
	if (numberOfChildren == 0) return "";
	index[0] = (char_t)(rand() % numberOfChildren + 'a');
	index[1] = '\0';

	string a = doc.child("speech").child(self->speechType).child(role).child(activity).child(index).text().as_string();
	delete[] index;
	return a;
}
CAI::~CAI()
{
}
bool CAI::IsPlayerInLineOfSight(int maxDistance, CEnemy * self)
{
	if (QuickDistanceToPlayer(self) < maxDistance)
	{
		//check if there are any tiles between player and enemy (from enemy eye-height (height/4) to player top and bottom)

		//first rule out tiles not between player and enemy
		int minX, maxX;
		int minY, maxY;
		if (self->GetX() > player->GetX())
		{
			minX = player->GetX() / vm->standardTileSize;
			maxX = self->GetX() / vm->standardTileSize;
		}
		else
		{
			minX = self->GetX() / vm->standardTileSize;
			maxX = player->GetX() / vm->standardTileSize;
		}
		if (self->GetY() > player->GetY())
		{
			minY = (player->GetY()) / vm->standardTileSize;
			maxY = self->GetY() / vm->standardTileSize;
		}
		else
		{
			minY = (self->GetY()) / vm->standardTileSize;
			maxY = player->GetY() / vm->standardTileSize;
		}

		//points to check
		int x1, x2, y1, y2;
		x1 = self->GetX() + self->GetWidth() / 2;
		y1 = self->GetY() + self->GetHeight() / 4;
		//loop through remaining tiles.
		//to optimize i could use an equation to find position in matrix of next tile

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				if (tileVector->at(y).at(x) != nullptr)
				{
					SDL_Rect rect;
					rect.x = x* vm->standardTileSize;
					rect.y = y* vm->standardTileSize;
					rect.w = vm->standardTileSize;
					rect.h = rect.w;

					x2 = player->GetX();
					y2 = player->GetY();
					if (SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
					{
						return false;
					}
				}
			}
		}
		return true; //return true if loop ends without returning false
	}
	else return false;

}
float CAI::GetOptimalXhairAngle(CEnemy* self)
{
	return atan2(vm->GetPlayer()->GetY() - self->GetY(), vm->GetPlayer()->GetX() - self->GetX());
}
CEnemy * CAI::GetEnemyClosestToPlayerFromVector(vector<CEnemy*> group)
{
	if (group.size() == 0) throw new std::exception("Empty group given to AI");
	CEnemy * closest = group[0];
	int distance = 2000;
	for (int i = 0; i < group.size(); i++)
	{
		if (QuickDistanceToPlayer(group[i]) < distance)
		{
			closest = group[i];
		}
	}
	return closest;
}
