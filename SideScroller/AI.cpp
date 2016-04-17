#include "AI.h"
#include "SlowMoveCommand.h"
#include <iostream>
#include <fstream>
#include "FileReader.h"
using std::cout;
using std::endl;
CAI::CAI(CVectorManager* _vm)
{
	vm = _vm;
	tileVector = vm->GetTileVector();
	player = vm->GetPlayer();
	sm = new CSpeechManager(vm);
}

void CAI::Idle(CEnemy* self)
{
	GetSpeechString(self, true);
	switch (self->currentIdleActivity)
	{
	case CEnemy::CurrentIdleActivity::idle:
		if (self->AITimer > 1000)
		{
			if (rand() % 40 < 8)
			{
				int newActivity = rand() % (int)CEnemy::CurrentIdleActivity::numberOfIdleActivites;
				cout << newActivity << endl;
				switch (newActivity)
				{
				case (int)CEnemy::CurrentIdleActivity::lookingLeft:
					self->ChangeAnimation(self->move);
					self->lookingLeft = true;
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingLeft;
					cout << "looking left" << endl;
					break;
				case (int)CEnemy::CurrentIdleActivity::lookingRight:
					self->ChangeAnimation(self->move);
					self->lookingLeft = false;
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingRight;
					cout << "looking right" << endl;
					break;
				case (int)CEnemy::CurrentIdleActivity::movingLeft:
					if (rand() % 2)
					{
						CCommandObject * cmd = new CSlowMoveCommand(self, 0);
						self->ChangeAnimation(self->move);
						self->lookingLeft = true;
						vm->AddObject(cmd);
						self->currentIdleActivity = CEnemy::CurrentIdleActivity::movingLeft;
						cout << "moving left" << endl;
						break;
					}
				case (int)CEnemy::CurrentIdleActivity::movingRight:
					if (rand() % 2)
					{
						CCommandObject * cmd = new CSlowMoveCommand(self, 1);
						vm->AddObject(cmd);
						self->ChangeAnimation(self->move);
						self->lookingLeft = false;
						self->currentIdleActivity = CEnemy::CurrentIdleActivity::movingRight;
						cout << "moving right" << endl;
						break;
					}
				default:
					cout << "idle rare" << endl;
					self->ChangeAnimation(self->idle_rare);
					break;
				}
			}
		}
		break;
	case CEnemy::CurrentIdleActivity::movingLeft:
	case CEnemy::CurrentIdleActivity::movingRight:

		if (self->AITimer > 800)
		{
			self->AITimer = 0;
			if (rand() % 2) break;
			CCommandObject * stop = new CSoftStopCommand(self);
			vm->AddObject(stop);
			self->ChangeAnimation(self->idle);
			self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
			cout << "soft stop" << endl;
		}
		break;

	case CEnemy::CurrentIdleActivity::lookingLeft:
		if (self->AITimer > 2000)
		{
			self->AITimer = 0;
			if (rand() % 2)
			{
				if (rand() % 3 > 0)
				{
					self->ChangeAnimation(self->idle);
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
					cout << "idle" << endl;
				}
				else
				{
					self->ChangeAnimation(self->move);
					self->lookingLeft = false;
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingRight;
					cout << "looking right" << endl;
					break;
				}
			}
		}
		break;
	case CEnemy::CurrentIdleActivity::lookingRight:
		if (self->AITimer > 1000)
		{
			self->AITimer = 0;
			if (rand() % 2)
			{
				if (rand() % 3 > 0)
				{
					self->ChangeAnimation(self->idle);
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::idle;
					cout << "idle" << endl;
				}
				else
				{
					self->ChangeAnimation(self->move);
					self->lookingLeft = true;
					self->currentIdleActivity = CEnemy::CurrentIdleActivity::lookingLeft;
					cout << "looking left" << endl;
					break;
				}
			}
		}
		break;
	}
}

std::string CAI::GetSpeechString(CEnemy * self, bool captain)
{
	pugi::xml_document doc;

	doc.load_file(vm->GetSpeechPath());

	char_t * activity = self->GetActivityString();
	vector<string> possibleLines;

	char_t * role;
	if (captain) role = "captain"; else role = "soldier";

	char_t * index = new char_t[2];
	int numberOfChildren = doc.child("speech").child(self->speechType).child(role).child(activity).child("amount").text().as_int();
	if (numberOfChildren == 0) return "";
	index[0] = (char_t)(rand() % numberOfChildren + 'a');
	index[1] = '\0';

	string a = doc.child("speech").child(self->speechType).child(role).child(activity).child(index).text().as_string();
	delete[] index;
	return a;
}
int CAI::DistanceToPlayer(CEnemy * self)
{
	return sqrt((player->GetX() - self->GetX()) * (player->GetX() - self->GetX()) + (player->GetY() - self->GetY()) * (player->GetY() - self->GetY()));
}
void CAI::Retreat(CEnemy* self)
{
	if (self->currentActivity != CEnemy::CurrentActivity::retreating)
	{
		self->currentActivity = CEnemy::CurrentActivity::retreating;
	}
	if (player->pos[0] > self->pos[0])
	{
		if (self->GetTimeUntilReaction() <= 0)
		{
			self->ResetTimeUntilReaction();

			CMoveCommand moveRight(self, 1);
			vm->DeleteObject(&moveRight);

			CCommandObject * moveLeft = new CMoveCommand(self, 0);
			vm->AddObject(moveLeft);

		}
		self->UpdateTimeUntilReaction(g_time);
	}
	else
	{
		if (self->GetTimeUntilReaction() <= 0)
		{
			self->ResetTimeUntilReaction();

			//remove left commands
			CMoveCommand moveLeft(self, 0);
			vm->DeleteObject(&moveLeft);

			//add right command
			CCommandObject * moveRight = new CMoveCommand(self, 1);
			vm->AddObject(moveRight);
		}
		self->UpdateTimeUntilReaction(g_time);
	}
	if (rand() % 300 + 1 == 29)
	{
		CJumpCommand * jump = new CJumpCommand(self);
		vm->AddObject(jump);
	}
}


void CAI::Attack(CEnemy* self)
{
	self->currentAnim = self->move;
	if (player->GetX() + player->GetWidth() < self->GetX() - 1)
	{
		CMoveCommand * move = new CMoveCommand(self, 0);
		vm->AddObject(move);
	}
	else if (player->GetX() > self->GetX() + self->GetWidth() + 1)
	{
		CMoveCommand * move = new CMoveCommand(self, 1);
		vm->AddObject(move);
	}
	if (DistanceToPlayer(self) < 1000)
	{
		if (self->GetPlayerVisible())
		{
			CAttackCommand * attack = new CAttackCommand(self);
			vm->AddObject(attack);
		}
		else
		{
			CCeaseFire * cease = new CCeaseFire(self);
			vm->AddObject(cease);
		}
	}
}

CAI::~CAI()
{
}

bool CAI::IsPlayerInLineOfSight(int maxDistance, CEnemy * self)
{
	if (DistanceToPlayer(self) < maxDistance)
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
		x1 = self->GetX();
		y1 = self->GetY();

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
