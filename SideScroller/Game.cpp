#include "Game.h"
#include "EntityFactory.h"
#include "MeleeWeapon.h"
#include "SoundEnum.h"
#include "GameSaver.h"
#include "HitscanWeapon.h"
#include "AIHandler.h"
#include "SimpleTestAI.h"
#include "RocketLauncher.h"
#include "EventManager.h"
#include "Hud.h"
#include "GroupManager.h"
#include "Group.h"

int g_time;
//global time slice variable

bool debugPause;
//debug tool


//game state global variable meanings:
// 0: playing
// 1: level complete
// 2: quit
// 4: die
//these control exits out of the game loop
int gameState;

CGame::CGame(string levelDirectory, bool * nextLevel, SDL_Renderer * renderer, SDL_Window * window, int resolutionX, int resolutionY, string saveName, int levelId)
{


	/////////////////////
	// BLESS THIS MESS //
	/////////////////////




	vm.standardTileSize = 64;
	vm.SetRenderPointer(renderer);
	CreateWeapons();

	// Create level from levelreade
	CGameSaver saver(&vm, saveName, levelId); //For loading and saving player data from/to textfile
	CLevelReader reader;
	reader.importLevel(levelDirectory + "level.txt");
	int xCounter = 0;
	int yCounter = 0;
	CEventManager eventTriggerManager;
	CSpeechManager speechManager(&vm);
	EntityFactory entityfactory(levelDirectory + "entitydata.txt", &vm, &speechManager);
	while (reader.NextTile())
	{
		if (reader.GetTileType() == 1)//block
		{
			CTile* tile = new CTile(xCounter, yCounter);
			vm.AddTile(tile);
			xCounter++;
		}
		else if (reader.GetTileType() == -1) //new row
		{
			vm.NewTileRow();
			yCounter++;
			xCounter = 0;
		}
		else if (reader.GetTileType() == 0) //non-block
		{
			vm.AddTile(nullptr); //represents an empty tile in the array
			xCounter++;
		}
		else if (reader.GetTileType() == 2) //non-player entity
		{
			xCounter++;
			vm.AddTile(nullptr); //represents an empty tile in the array

			if (entityfactory.CreateEntity(reader.entityId.at(0), (xCounter * vm.standardTileSize) - vm.standardTileSize, yCounter * vm.standardTileSize)) // creates an object. stores the object type in currentType and the pointer in the corresponding holder. returns true if successful
			{
				switch (entityfactory.currentType)
				{
				case entityfactory.enemy:

					vm.AddObject(entityfactory.enemyHolder);
					break;

				case entityfactory.trap:

					vm.AddObject(entityfactory.trapHolder);
					break;

				case entityfactory.crate:

					vm.AddObject(entityfactory.crateHolder);
					break;

				case entityfactory.chest:

					vm.AddObject(entityfactory.chestHolder);
					break;
				case entityfactory.eventTrigger:
					eventTriggerManager.AddEventTrigger(entityfactory.eventHolder);
					break;
				case entityfactory.backgroundObject:
					vm.AddObject(entityfactory.backgroundHolder);
					break;
				default:
					break;
				}
			}
		}
		else if (reader.GetTileType() == 3) //player
		{
			xCounter++;
			vm.AddTile(nullptr);

			CPlayer * player = saver.LoadGame(xCounter * vm.standardTileSize, yCounter * vm.standardTileSize, 0, vm.standardTileSize);
			vm.AddObject(player);
		}
	}
	
	map<int, CGroup*>* groupMap = entityfactory.groupMap;

	//Calculate textures
	int counter = 0;

	for (int row = 0; row < vm.GetTileVector()->size(); row++)
	{
		for (int col = 0; col < vm.GetTileVector()->at(row).size(); col++)
		{
			counter++;
			if (vm.GetTileVector()->at(row).at(col) != nullptr)
			{
				vm.GetTileVector()->at(row).at(col)->CalculateProperties(*vm.GetTileVector());
			}
		}
	}


	

	//Create handlers
	ProjectileHandler projectileHandler(&vm);
	CDialogHandler dialogHandler(&vm, resolutionX, resolutionY);
	CInputHandler inputHandler(&vm, &dialogHandler);
	CPhysicsHandler physicsHandler(&vm);
	CArtist picasso(&vm, 0, resolutionX, resolutionY, levelDirectory + "texturedata.txt", &dialogHandler, renderer, window);
	CGroupManager groups(groupMap, &vm);
	CTrapHandler traphandler(&vm);
	CDeathManager death(&vm, groupMap);
	eventTriggerManager.SetArtistPointer(&picasso);
	
	///
	CBackgroundObject * bgobj = new CBackgroundObject(5540, 704, 2003, 512, 192, 1);
	vm.AddObject(bgobj);
	CBackgroundObject * bgobj2 = new CBackgroundObject(5540, 704, 2004, 128, 128, 1);
	vm.AddObject(bgobj2);
	CBackgroundObject * dirt1 = new CBackgroundObject(1473, 2100, 2005, 512, 512, 0.96f);
	vm.AddObject(dirt1);
	///


	InitTimer();

	int frames=0;
	int ms = 0;

	//Game loop
	gameState = 0;
	while (gameState == 0 || gameState == 3)
	{
		
		frames++;
		ms += g_time;
		if (ms > 500)
		{
			std::cout << vm.GetPlayer()->GetX() << std::endl;
			std::cout << vm.GetPlayer()->GetY() << std::endl;
			std::cout << "----" << std::endl;
			frames = 0;
			ms = 0;
		}
		SDL_Delay(4);
		g_time = GetTicks();
		dialogHandler.GetInventory()->Update();
		dialogHandler.Update();
		inputHandler.HandleInput();

		if (!dialogHandler.IsDialogOpen())
		{
			groups.HandleGroups();
			speechManager.UpdateSpeech();
			physicsHandler.DoPhysics();

			ExecuteCommands();
			ExecuteEffects();
			eventTriggerManager.HandleEvents();
			for (int b = 0; b < vm.GetBaseVector()->size(); b++)
			{
				vm.GetBaseVector()->at(b)->UpdateTime(g_time);
			}

			death.UpdateCrates();
			death.UpdateAnimates();

			projectileHandler.HandleProjectiles();

			traphandler.HandleTraps();


			for (int c = 0; c < vm.GetWeaponVector()->size(); c++)
			{
				vm.GetWeaponVector()->at(c)->Update();
			}

		}
		picasso.DrawEverything();
	}
	if (gameState == 1) //if global variable gamestate is 1 the player has completed the level
	{

		*nextLevel = true; //then we set CCampaigns bool nextLevel to true, which means that CCampaign will load the next level
	
		saver.SaveGame();
	}
	if (gameState == 3)	 
	{
		*nextLevel = false;
	}
	vm.DeleteAllObjects();



}



void CGame::ExecuteCommands()
{
	vector<CCommandObject*> * commandVector = vm.GetCommandVector();
	int commandAmount = commandVector->size();
	for (int c = 0; c < commandAmount; c++)
	{
		if (commandVector->at(c)->Execute())
		{
			vm.DeleteObject(commandVector->at(c));
			c--;
			commandAmount--;
		}
	}
}

void CGame::ExecuteEffects()
{
	vector<CEffect*>* effectVector = vm.GetEffectVector();
	int effectAmount = effectVector->size();
	for (int c = 0; c < effectAmount; c++)
	{
		if (effectVector->at(c)->Execute())
		{
			vm.DeleteObject(effectVector->at(c));
			c--;
			effectAmount--;
		}
	}
}

int CGame::GetTicks()
{
	int tempTotal = SDL_GetTicks();
	int slice = tempTotal - totalMs;

	totalMs = tempTotal;
	return slice;
}

void CGame::InitTimer()
{
	totalMs = SDL_GetTicks();
}

CGame::~CGame()
{
	vm.DeleteAllObjects();
}

void CGame::CreateWeapons()
{
	//Create weapons
	CMeleeWeapon* noWep = new CMeleeWeapon(&vm);
	noWep->animPointOfNoReturnInMs = 25;
	vm.AddObject(noWep);
	CWeapon* grenadeLauncher = new CGrenadeLauncher(&vm);
	vm.AddObject(grenadeLauncher);
	//CHitscanWeapon* hitscan = new CHitscanWeapon(&vm);
	//vm.AddObject(hitscan);
	CRocketLauncher* rocket = new CRocketLauncher(&vm);
	vm.AddObject(rocket);
	vm.AddObject(rocket);

	CBulletWeapon* shotgun = new CBulletWeapon(&vm);
	shotgun->clipSize = 8;
	shotgun->damage = 4;
	shotgun->numberOfPellets = 8;
	shotgun->fireConeTightness = 100000.0f;
	shotgun->maxLength = 1000;
	shotgun->tracerIntensity = 0.5f;
	shotgun->fireDelay = 1000;
	shotgun->reloadDelay = 3500;
	shotgun->iconId = 205;
	shotgun->textureId = 105;
	shotgun->flipPoint.y = 5;
	shotgun->width = 70;
	shotgun->height = 9;
	shotgun->offsetX = 70;
	shotgun->kickBackRot = -15;
	shotgun->kickBackTime = 250;
	shotgun->name = "Shotgun";
	shotgun->fireSound = shotgun_blast;
	vm.AddObject(shotgun);

	CBulletWeapon* heavyShotgun = new CBulletWeapon(&vm);
	heavyShotgun->clipSize = 4;
	heavyShotgun->damage = 12;
	heavyShotgun->numberOfPellets = 9;
	heavyShotgun->fireConeTightness = 14000.0f;
	heavyShotgun->maxLength = 1000;
	heavyShotgun->tracerIntensity = 1.1f;
	heavyShotgun->fireDelay = 1200;
	heavyShotgun->reloadDelay = 2500;
	heavyShotgun->textureId = 105;
	heavyShotgun->width = 90;
	heavyShotgun->height = 11;
	heavyShotgun->offsetX = 90;
	heavyShotgun->kickBackX = -12;
	heavyShotgun->kickBackRot = -25;
	heavyShotgun->kickBackTime = 500;
	heavyShotgun->iconId = 5;
	heavyShotgun->semiAutomatic = true;
	heavyShotgun->name = "Heavy Shotgun";
	heavyShotgun->fireSound = heavyShotgun_blast;
	vm.AddObject(heavyShotgun);

	CBulletWeapon* glock = new CBulletWeapon(&vm);
	glock->clipSize = 20;
	glock->damage = 5;
	glock->numberOfPellets = 1;
	glock->fireConeTightness = 60000.0f;
	glock->maxLength = 1300;
	glock->tracerIntensity = 0.3f;
	glock->fireDelay = 100;
	glock->reloadDelay = 2000;
	glock->height = 10;
	glock->width = 25;
	glock->offsetX = 43;
	glock->offsetY = 12;
	glock->kickBackX = -2;
	glock->kickBackRot = -5;
	glock->kickBackTime = 100;
	glock->iconId = 6;
	glock->semiAutomatic = true;
	glock->name = "Glock-18";
	glock->fireSound = glock_shot;
	vm.AddObject(glock);

	CBulletWeapon* ak = new CBulletWeapon(&vm);
	ak->clipSize = 30;
	ak->damage = 10;
	ak->numberOfPellets = 1;
	ak->fireConeTightness = 80000.0f;
	ak->maxLength = 1300;
	ak->tracerIntensity = 0.4f;
	ak->fireDelay = 100;
	ak->reloadDelay = 2000;
	ak->offsetX = 65;
	ak->offsetY = 0;
	ak->kickBackX = -6;
	ak->kickBackY = -1;
	ak->kickBackRot = -3;
	ak->kickBackTime = 200;
	ak->iconId = 201;
	ak->semiAutomatic = false;
	ak->recoilIncreasePerBullet = 0.05f;
	ak->spreadIncreasePerBullet = 7000;
	ak->maxExtraRecoil = 0.18f;
	ak->maxExtraSpread = 50000;
	ak->textureId = 101;
	ak->width =65;
	ak->height = 15;
	ak->flipPoint.y = 7;
	ak->name = "AK-47";
	ak->fireSound = ak47_shot;
	vm.AddObject(ak);
	CBulletWeapon* m4a4 = new CBulletWeapon(&vm);
	m4a4->clipSize = 30;
	m4a4->damage = 7;
	m4a4->numberOfPellets = 1;
	m4a4->fireConeTightness = 110000.0f;
	m4a4->maxLength = 1300;
	m4a4->tracerIntensity = 0.2f;
	m4a4->fireDelay = 95;
	m4a4->reloadDelay = 2000;
	m4a4->height = 24	;
	m4a4->width = 71;
	m4a4->offsetX = 71;
	m4a4->offsetY = 0;
	m4a4->kickBackY = -2;
	m4a4->kickBackX = -2;
	m4a4->kickBackRot = -3;
	m4a4->kickBackTime = 95;
	m4a4->iconId = 1;
	m4a4->semiAutomatic = false;
	m4a4->recoilIncreasePerBullet = 0.045f;
	m4a4->spreadIncreasePerBullet = 6000;
	m4a4->maxExtraRecoil = 0.1f;
	m4a4->maxExtraSpread = 40000;	
	m4a4->name = "M4A4";
	m4a4->textureId = 104;
	m4a4->iconId = 204;
	m4a4->flipPoint.y = 12;
	m4a4->fireSound = silenced_shot;
	vm.AddObject(m4a4);
	

	CBulletWeapon* mp5 = new CBulletWeapon(&vm);
	mp5->clipSize = 30;
	mp5->damage = 3;
	mp5->numberOfPellets = 1;
	mp5->fireConeTightness = 110000.0f;
	mp5->maxLength = 1300;
	mp5->tracerIntensity = 0.3f;
	mp5->fireDelay = 100;
	mp5->reloadDelay = 2000;
	mp5->height = 17;
	mp5->width = 49;
	mp5->offsetX = 49;
	mp5->offsetY = 3;
	mp5->kickBackY = -2;
	mp5->kickBackX = -2;
	mp5->kickBackRot = -3;
	mp5->kickBackTime = 95;
	mp5->iconId = 202;
	mp5->textureId = 102;
	mp5->flipPoint.y = 8;
	mp5->semiAutomatic = false;
	mp5->name = "MP5";
	mp5->fireSound = mg_shot;
	vm.AddObject(mp5);

	CMeleeWeapon* spear_mudhands = new CMeleeWeapon(&vm);
	spear_mudhands->textureId = 601;
	spear_mudhands->animationData[spear_mudhands->idle][spear_mudhands->frameCountData] = 1;
	spear_mudhands->animationData[spear_mudhands->idle][spear_mudhands->msPerFrameData] = 700;
	spear_mudhands->animationData[spear_mudhands->idle][spear_mudhands->startFrameData] = 0;
	spear_mudhands->animationData[spear_mudhands->attack1][spear_mudhands->frameCountData] = 11;
	spear_mudhands->animationData[spear_mudhands->attack1][spear_mudhands->msPerFrameData] = 45;
	spear_mudhands->animationData[spear_mudhands->attack1][spear_mudhands->startFrameData] = 1;
	spear_mudhands->animationData[spear_mudhands->attack2][spear_mudhands->frameCountData] = 0;
	spear_mudhands->animationData[spear_mudhands->attack2][spear_mudhands->msPerFrameData] = 25;
	spear_mudhands->animationData[spear_mudhands->attack2][spear_mudhands->startFrameData] = 1;
	spear_mudhands->fireDelay = 1000;
	spear_mudhands->flipPoint.x = 8;
	spear_mudhands->flipPoint.y = 12;
	spear_mudhands->timeSinceAnimationStart = 0;
	spear_mudhands->frameSizeX = 160;
	spear_mudhands->frameSizeY = 24;
	spear_mudhands->height = 24;
	spear_mudhands->width = 160;
	spear_mudhands->maxLength = 128;
	spear_mudhands->offsetX = 32;
	spear_mudhands->offsetY = 10;
	spear_mudhands->damage = 20;
	spear_mudhands->ammoType = melee;
	spear_mudhands->ammoCost = 0;
	spear_mudhands->staminaCost = 8;
	spear_mudhands->animPointOfNoReturnInMs = 150;
	spear_mudhands->positionAdjustment[0] = -7;
	spear_mudhands->positionAdjustment[1] = -3;
	spear_mudhands->idleRotation = -90;
	spear_mudhands->idlePositionAdjustment[0] = 50;
	spear_mudhands->idlePositionAdjustment[1] = 40;
	vm.AddObject(spear_mudhands);


}