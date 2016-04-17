#include "EntityFactory.h"


EntityFactory::EntityFactory(string path, CVectorManager *_vm, CSpeechManager* _smgr)
{
	entityDataPath = path;
	vm = _vm;
	groupMap = new map<int, CGroup*>();
	smgr = _smgr;
}


EntityFactory::~EntityFactory()
{
}

bool EntityFactory::CreateEntity(char id, int x, int y)
{
	std::fstream reader(entityDataPath);
	string wholeFile;

	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		wholeFile += temp;
	}
	reader.close();
	for (int i = 0; i < wholeFile.length(); i++)
	{
		string row = GetRow(wholeFile, i);
		if (row == "eof")
		{
			//matching id was not found
			return false;
		}
		if (row.length() > 0)
		{
			if (row.at(0) == '[' && row.at(1) == id)
			{
				string typeRow = GetRow(wholeFile, i + 1);
				//get type
				if (typeRow == "eof")
				{
					//eof before typerow
					return false;
				}
				else if (typeRow == "enemy")
				{
					enemyHolder = CreateEnemy(wholeFile, i + 2, x, y);
					if (enemyHolder == nullptr)
					{
						//if CreateEnemy fails
						return false;
					}
					else
					{
						//if CreateEnemy succeeds
						currentType = enemy;
						return true;
					}
				}
				else if (typeRow == "trap")
				{
					trapHolder = CreateTrap(wholeFile, i + 2, x, y);
					if (trapHolder == nullptr)
					{
						return false;
					}
					else
					{
						currentType = trap;
						return true;
					}
				}
				else if (typeRow == "crate")
				{
					crateHolder = CreateCrate(wholeFile, i + 2, x, y);
					if (crateHolder == nullptr)
					{
						return false;
					}
					else
					{
						currentType = crate;
						return true;
					}
				}
				else if (typeRow == "chest")
				{
					chestHolder = CreateChest(wholeFile, i + 2, x, y);
					if (chestHolder == nullptr)
					{
						return false;
					}
					else
					{
						currentType = chest;
						return true;
					}
				}
				else if (typeRow == "eventtrigger")
				{
					eventHolder = CreateEvent(wholeFile, i + 2, x, y);
					if (eventHolder == nullptr)
					{
						return false;
					}
					else
					{
						currentType = eventTrigger;
						return true;
					}
				}
				else if (typeRow == "background")
				{
					backgroundHolder = CreateBackgroundObject(wholeFile, i + 2, x, y);
					if (backgroundHolder == nullptr) return false;
					else
					{
						currentType = backgroundObject;
						return true;
					}
				}
			}
		}
	}
	return false;
}

CEventTrigger* EntityFactory::CreateEvent(const string & file, int startRow, int x, int y)
{
	
	switch (atoi(GetRow(file, startRow).c_str()))
	{
	case 0: //zone
	{
		CEventTriggerZone * eventTrigger = new CEventTriggerZone(vm, atoi(GetRow(file, startRow + 1).c_str()));
		eventTrigger->SetArea(x + atoi(GetRow(file, startRow + 2).c_str()), y + atoi(GetRow(file, startRow + 3).c_str()), atoi(GetRow(file, startRow + 4).c_str()), atoi(GetRow(file, startRow + 5).c_str()));
		eventTrigger->eventId = atoi(GetRow(file, startRow + 6).c_str());
		return eventTrigger;
		break;
	}
	default:
		return nullptr; //can happen if invalid event id is entered in entitydata.txt
	}
}

CTrapObject* EntityFactory::CreateTrap(const string & file, int startRow, int x, int y)
{
	CTrapObject* trap = new CFluid(x, y, vm, atof(GetRow(file, startRow + 9).c_str()));
	//get physics data

	trap->textureId = atoi(GetRow(file, startRow).c_str());
	trap->length = atoi(GetRow(file, startRow + 1).c_str());
	trap->height = atoi(GetRow(file, startRow + 2).c_str());
	trap->animationData[trap->idle][trap->frameCountData] = atoi(GetRow(file, startRow + 3).c_str());
	trap->animationData[trap->idle][trap->msPerFrameData] = atoi(GetRow(file, startRow + 4).c_str());
	trap->animationData[trap->death][trap->frameCountData] = atoi(GetRow(file, startRow + 5).c_str());
	trap->animationData[trap->death][trap->msPerFrameData] = atoi(GetRow(file, startRow + 6).c_str());
	trap->frameSizeX = atoi(GetRow(file, startRow + 7).c_str());
	trap->frameSizeY = atoi(GetRow(file, startRow + 8).c_str());

	trap->animationData[trap->idle][trap->startFrameData] = 0;
	trap->animationData[trap->death][trap->startFrameData] = trap->animationData[trap->idle][trap->frameCountData];

	return trap;
}

CChest * EntityFactory::CreateChest(const string & file, int startRow, int x, int y)
{
	CChest* chest = new CChest();
	chest->pos[0] = x;
	chest->pos[1] = y;
	chest->textureId = atoi(GetRow(file, startRow).c_str());
	chest->SetContent(atoi(GetRow(file, startRow + 1).c_str()));
	chest->SetKey(atoi(GetRow(file, startRow + 2).c_str()));
	chest->animationData[chest->locked][chest->frameCountData] = atoi(GetRow(file, startRow + 3).c_str());
	chest->animationData[chest->locked][chest->msPerFrameData] = atoi(GetRow(file, startRow + 4).c_str());
	chest->animationData[chest->opened][chest->frameCountData] = atoi(GetRow(file, startRow + 5).c_str());
	chest->animationData[chest->opened][chest->msPerFrameData] = atoi(GetRow(file, startRow + 6).c_str());

	chest->animationData[chest->locked][chest->startFrameData] = 0;
	chest->animationData[chest->opened][chest->startFrameData] = chest->animationData[chest->locked][chest->frameCountData];
	return chest;
}
CCrate* EntityFactory::CreateCrate(const string & file, int startRow, int x, int y)
{
	CCrate* crate = new CCrate(x, y);
	//get physics data


	crate->textureId = atoi(GetRow(file, startRow).c_str());
	crate->length = atoi(GetRow(file, startRow + 1).c_str());
	crate->height = atoi(GetRow(file, startRow + 2).c_str());
	crate->SetContent(atoi(GetRow(file, startRow + 3).c_str()));
	crate->animationData[crate->idle][crate->frameCountData] = atoi(GetRow(file, startRow + 4).c_str());
	crate->animationData[crate->idle][crate->msPerFrameData] = atoi(GetRow(file, startRow + 5).c_str());
	crate->animationData[crate->death][crate->frameCountData] = atoi(GetRow(file, startRow + 6).c_str());
	crate->animationData[crate->death][crate->msPerFrameData] = atoi(GetRow(file, startRow + 7).c_str());
	crate->frameSizeX = atoi(GetRow(file, startRow + 8).c_str());
	crate->frameSizeY = atoi(GetRow(file, startRow + 9).c_str());



	crate->animationData[crate->idle][crate->startFrameData] = 0;
	crate->animationData[crate->death][crate->startFrameData] = crate->animationData[crate->idle][crate->frameCountData];


	return crate;
}
CEnemy* EntityFactory::CreateEnemy(const string & file, int startRow, int x, int y)
{
	CEnemy* enemy = new CEnemy(x, y, 0, 3, *vm->GetWeaponVector());
	//get physics data

	enemy->textureId = atoi(GetRow(file, startRow).c_str());
	enemy->length = atoi(GetRow(file, startRow + 1).c_str());
	enemy->height = atoi(GetRow(file, startRow + 2).c_str());
	enemy->SetHP(atoi(GetRow(file, startRow + 3).c_str()));
	enemy->weight = atof(GetRow(file, startRow + 4).c_str());
	enemy->jumpStrength = atof(GetRow(file, startRow + 5).c_str());
	enemy->moveSpeed = atof(GetRow(file, startRow + 6).c_str());
	enemy->maxMoveSpeed = atof(GetRow(file, startRow + 7).c_str());
	enemy->SetLoot(atoi(GetRow(file, startRow + 8).c_str()));

	// get animationdata
	enemy->animationData[enemy->idle][enemy->frameCountData] = atoi(GetRow(file, startRow + 9).c_str());
	enemy->animationData[enemy->idle][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 10).c_str());

	enemy->animationData[enemy->idle_rare][enemy->frameCountData] = atoi(GetRow(file, startRow + 11).c_str());
	enemy->animationData[enemy->idle_rare][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 12).c_str());

	enemy->animationData[enemy->move][enemy->frameCountData] = atoi(GetRow(file, startRow + 13).c_str());
	enemy->animationData[enemy->move][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 14).c_str());

	enemy->animationData[enemy->attack][enemy->frameCountData] = atoi(GetRow(file, startRow + 15).c_str());
	enemy->animationData[enemy->attack][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 16).c_str());

	enemy->animationData[enemy->dmg][enemy->frameCountData] = atoi(GetRow(file, startRow + 17).c_str());
	enemy->animationData[enemy->dmg][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 18).c_str());

	enemy->animationData[enemy->death][enemy->frameCountData] = atoi(GetRow(file, startRow + 19).c_str());
	enemy->animationData[enemy->death][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 20).c_str());

	enemy->animationData[enemy->jump][enemy->frameCountData] = atoi(GetRow(file, startRow + 21).c_str());
	enemy->animationData[enemy->jump][enemy->msPerFrameData] = atoi(GetRow(file, startRow + 22).c_str());

	///////////////
	// calculate startframes
	int runningTotal = 0;
	enemy->animationData[enemy->idle][enemy->startFrameData] = 0;
	for (int i = 0; i < enemy->numberOfAnimations; i++)
	{
		enemy->animationData[i][enemy->startFrameData] = runningTotal;
		runningTotal += enemy->animationData[i][enemy->frameCountData];
	}
	///////////////
	//size of frames in texturefile
	enemy->frameSizeX = atoi(GetRow(file, startRow + 23).c_str());
	enemy->frameSizeY = enemy->frameSizeX;
	//attack and dmg data - i speculate that enemy will use the same kind of weapons that are laying around in the maps. it would make sense.
	enemy->SetWeaponEquipped(atoi(GetRow(file, startRow + 24).c_str()));
	//ai stuff
	enemy->AI = atoi(GetRow(file, startRow + 25).c_str());
	enemy->aiParam[0] = atoi(GetRow(file, startRow + 26).c_str());
	enemy->aiParam[1] = atoi(GetRow(file, startRow + 27).c_str());
	enemy->aiParam[2] = atoi(GetRow(file, startRow + 28).c_str());
	enemy->aiParam[3] = atoi(GetRow(file, startRow + 29).c_str());
	int groupNumber = atoi(GetRow(file, startRow + 30).c_str());
	int captain = atoi(GetRow(file, startRow + 31).c_str());
	enemy->groupNumber = groupNumber;
	if (groupMap->count(groupNumber) == 0)
	{
		CGroup * group = new CGroup();
		group->AddMember(enemy, captain);
		group->speechManager = smgr;
		groupMap->insert(std::pair<int, CGroup*>(groupNumber, group));
	}
	else
	{
		groupMap->at(groupNumber)->AddMember(enemy, captain);
	}
	enemy->bravery = atoi(GetRow(file, startRow + 32).c_str());
	return enemy;
}
CBackgroundObject* EntityFactory::CreateBackgroundObject(const string & file, int startRow, int x, int y)
{
	CBackgroundObject * obj = new CBackgroundObject(x, y, atoi(GetRow(file, startRow).c_str()), atoi(GetRow(file, startRow + 1).c_str()), atoi(GetRow(file, startRow + 2).c_str()), atof(GetRow(file, startRow + 3).c_str()));
	obj->frameSizeX = atoi(GetRow(file, startRow + 4).c_str());
	obj->frameSizeY = atoi(GetRow(file, startRow + 5).c_str());
	obj->msPerFrame = atoi(GetRow(file, startRow + 6).c_str());
	obj->numberOfFrames = atoi(GetRow(file, startRow + 7).c_str());
	return obj;
}
string EntityFactory::GetRow(const string & file, int rownumber)
{
	int rowCounter = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (rowCounter == rownumber)
		{
			int counter = 0;
			int tally = 0;
			string row;
			while (true)
			{
				if (i + counter < file.size())
				{
					if (file.at(i + counter) != '\n')
					{
						counter++;
					}
					else
					{
						row = file.substr(i, counter);
						return row;
					}
				}
				else return "eof";
			}
		}
		if (file.at(i) == '\n')
		{
			rowCounter++;
		}
	}
	return "eof";
}