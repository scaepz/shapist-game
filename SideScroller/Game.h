#ifndef GAME_H
#define GAME_H

#include <vector>
#include "DialogHandler.h"
#include "InputHandler.h"
#include "CommandObject.h"
#include "Player.h"
#include "LevelReader.h"
#include "Tile.h"
#include "StaticObject.h"
#include "Artist.h"
#include "PhysicsHandler.h"
#include "ProjectileHandler.h"
#include "Enemy.h"
#include "Weapon.h"
#include "GrenadeLauncher.h"
#include "VectorManager.h"
#include "Effect.h"
#include "TrapObject.h"
#include "TrapHandler.h"
#include "DeathManager.h"
#include "BulletWeapon.h"

using std::vector;
class CGame
{
public:
	
	CGame(string levelDirectory, bool * nextLevel, SDL_Renderer *, SDL_Window *, int resolutionX, int resolutionY, string saveName, int levelId);
	virtual ~CGame();
	
private:

	int totalMs;
//	const int standardTileSize = 64;

	///
	//relics from experiment with circletype coll. det.
	struct Circle
	{
		int x, y, r;
	};
	bool Intersects(Circle circle, SDL_Rect rect);
	///
	void InitTimer();
	int GetTicks();
	void CreateWeapons();
	CVectorManager vm;
	void ExecuteCommands();
	void ExecuteEffects();
};

#endif