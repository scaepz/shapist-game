#ifndef VECTORMANAGER_H
#define VECTORMANAGER_H
#include "Animate.h"
#include "Tile.h"
#include "TrapObject.h"
#include "Chest.h"
#include "Enemy.h"
#include "CommandObject.h"
#include "Player.h"
#include "Effect.h"
#include "Projectile.h"
#include "Crate.h"
#include "Explosion.h"
#include "BackgroundObject.h"
#include "DamageNumber.h"
#include "Item.h"
#include "SoundPlayer.h"
#include <vector>
#include "ParticleEngine.h"
#include "Speech.h"
class CVectorManager
{
public:
	CVectorManager();
	virtual ~CVectorManager();

	void AddObject(CBackgroundObject*);
	void AddObject(CDamageNumber*);
	void AddObject(CProjectile*);
	void AddObject(CPlayer*);
	void AddObject(CEnemy*);
	void AddObject(CTrapObject*);
	void AddObject(CExplosion*);
	void AddObject(CWeapon*);
	void AddObject(CCommandObject*);
	void AddObject(CEffect*);
	void AddObject(CCrate*);
	void AddObject(CItem*);
	void AddObject(CChest*);
	void AddObject(CSpeech*);
	void DeleteObjectsConcerning(CBaseObject * obj); //Delete commands and effects linked to object

	
	void AddTile(CTile*);
	void NewTileRow();
	int standardTileSize;
	
	void DeleteObject(CBackgroundObject*);
	void DeleteObject(CDamageNumber*);
	void DeleteObject(CEffect*); //Do not create a new dynamic object to send here. This method works by finding all identical objects and deleting them, not by deleting via address. //
	void DeleteObject(CCommandObject*); //Do not create a new dynamic object to send here. This method works by finding all identical objects and deleting them, not by deleting via address.
	void DeleteObject(CBaseObject* obj);
	void DeleteAllObjects();

	CPlayer* GetPlayer();

	vector < vector<CTile*> >* GetTileVector();

	vector <CPhysicsObject*>* GetPhysicsVector();
	vector <CBaseObject*>* GetBaseVector();

	vector <CProjectile*>* GetProjectileVector();
	vector <CWeapon*>* GetWeaponVector();

	vector <CBaseObject*>* GetSolidVector(); //physicsobjects can collide into these (and be these). Only passthrough objects should not be these? idk dammit
	vector <CAnimate*>* GetAnimateVector();
	vector <CEnemy*>* GetEnemyVector();
	vector<CDamageNumber*>* GetDamageNumberVector();
	vector <CTrapObject*>* GetTrapVector();
	vector <CCrate*>* GetCrateVector();
	vector <CCommandObject*>* GetCommandVector();
	vector <CEffect*>* GetEffectVector();
	vector<CExplosion*>* GetExplosionVector();
	vector<CItem*>* GetItemVector();
	vector<CChest*>* GetChestVector();
	vector<CBackgroundObject*>* GetBackgroundVector();
	CParticleEngine * GetParticleEngine();
	CSoundPlayer * GetSoundPlayer();
	vector<CSpeech*>* GetSpeechVector();
	SDL_Renderer * GetRenderer();
	void SetRenderPointer(SDL_Renderer*);
private:

	SDL_Renderer * renderer;
	CParticleEngine particleEngine;
	CSoundPlayer soundPlayer;
	CPlayer* player = nullptr;
	vector < vector<CTile*> > tileVector;
	vector <CTile*> tRow;
	vector<CSpeech*> speechVector;
	vector<CBackgroundObject*> backgroundVector;
	vector<CDamageNumber*> damageNumberVector;
	vector<CChest*> chestVector;
	vector<CExplosion*> explosionVector;
	vector <CPhysicsObject*> physicsVector;
	vector <CBaseObject*> baseVector;
	vector <CProjectile*> projectileVector;
	vector <CWeapon*> weaponVector;
	vector <CBaseObject*> solidVector; //physicsobjects can collide into these (and be these)
	vector <CAnimate*> animateVector;
	vector <CEnemy*> enemyVector;
	vector <CCrate*> crateVector;
	vector <CTrapObject*> trapVector;

	vector <CCommandObject*> commandVector;
	vector <CEffect*> effectVector;
	vector<CItem*> itemVector;
};


#endif