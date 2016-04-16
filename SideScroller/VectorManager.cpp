#include "VectorManager.h"
#include "SoundEnum.h"
#include "MoveCommand.h"
#include "SlowMoveCommand.h"
CVectorManager::CVectorManager()
{

}


CVectorManager::~CVectorManager()
{
}

CSoundPlayer * CVectorManager::GetSoundPlayer()
{
	return &soundPlayer;
}
CParticleEngine * CVectorManager::GetParticleEngine()
{
	return &particleEngine;
}
void CVectorManager::DeleteAllObjects() //including tiles
{

	//delete base
	while (baseVector.size() > 0)
	{
		delete baseVector[0];
		baseVector.erase(baseVector.begin());
	}
	//delete damage numbers
	while (damageNumberVector.size() > 0)
	{
		delete damageNumberVector[0];
		damageNumberVector.erase(damageNumberVector.begin());
	}
	//delete effects
	while (effectVector.size() > 0)
	{
		delete effectVector[0];
		effectVector.erase(effectVector.begin());
	}
	//delete commands
	while (commandVector.size() > 0)
	{
		delete commandVector[0];
		commandVector.erase(commandVector.begin());
	}
	//delete weapons
	while (weaponVector.size() > 0)
	{
		delete weaponVector[0];
		weaponVector.erase(weaponVector.begin());
	}
	while (backgroundVector.size() > 0)
	{
		delete backgroundVector[0];
		backgroundVector.erase(backgroundVector.begin());
	}
	//delete tiles
	while (tileVector.size() > 0)
	{
		while (tileVector.at(0).size() > 0)
		{
			delete tileVector.at(0).at(0);
			tileVector.at(0).erase(tileVector.at(0).begin());
		}
		tileVector.erase(tileVector.begin());
	}
	//delete particles
	particleEngine.DeleteAllParticles();
	
	//that all?

}

void CVectorManager::DeleteObject(CDamageNumber* obj)
{
	for (int i = 0; i < damageNumberVector.size(); i++)
	{
		if (damageNumberVector[i] == obj)
		{
			damageNumberVector.erase(damageNumberVector.begin() + i);
			i--;
		}
	}
}

void CVectorManager::DeleteObject(CBaseObject *obj)
{
	//erase object from all vectors, one at a time because i cant figure out how to make a collection of multiple types

	bool animate = false; //used to slightly optimize by
	for (int i = 0; i < baseVector.size(); i++)
	{
		if (baseVector[i] == obj)
		{
			baseVector.erase(baseVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < physicsVector.size(); i++)
	{
		if (physicsVector[i] == obj)
		{
			physicsVector.erase(physicsVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < animateVector.size(); i++)
	{
		if (animateVector[i] == obj)
		{
			animate = true;
			animateVector.erase(animateVector.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < solidVector.size(); i++)
	{
		if (solidVector[i] == obj)
		{
			solidVector.erase(solidVector.begin() + i);
			i--;
		}
	}

	if (animate)
	{

		DeleteObjectsConcerning(obj);

		for (int i = 0; i < enemyVector.size(); i++)
		{
			if (enemyVector[i] == obj)
			{
				enemyVector.erase(enemyVector.begin() + i);
				i--;
			}
		}
	}
	else
	{
		for (int i = 0; i < chestVector.size(); i++)
		{
			if (chestVector[i] == obj)
			{
				chestVector.erase(chestVector.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < itemVector.size(); i++)
		{
			if (itemVector[i] == obj)
			{
				itemVector.erase(itemVector.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < trapVector.size(); i++)
		{
			if (trapVector[i] == obj)
			{
				trapVector.erase(trapVector.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < projectileVector.size(); i++)
		{
			if (projectileVector[i] == obj)
			{
				projectileVector.erase(projectileVector.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < explosionVector.size(); i++)
		{
			if (explosionVector[i] == obj)
			{
				explosionVector.erase(explosionVector.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < crateVector.size(); i++)
		{
			if (crateVector[i] == obj)
			{
				crateVector.erase(crateVector.begin() + i);
				i--;
			}
		}

	}
	delete obj;
}

CPlayer* CVectorManager::GetPlayer()
{
	return player;
}
void CVectorManager::AddObject(CEffect* effect)
{
	bool effectAlreadyExists = false;


	for (int i = 0; i < effectVector.size(); i++)
	{
		if (effect->obj == effectVector.at(i)->obj && effect->effectId == effectVector.at(i)->effectId)
		{
			effectAlreadyExists = true;
			effectVector.at(i)->timeSinceExecute = 0;
			break;
		}
	}
	if (!effectAlreadyExists)
	{
		effectVector.push_back(effect);
	}
	else delete effect;
}
vector<CBackgroundObject*>* CVectorManager::GetBackgroundVector()
{
	return &backgroundVector;
}
void CVectorManager::AddObject(CBackgroundObject* obj)
{
	backgroundVector.push_back(obj);
}
void CVectorManager::DeleteObject(CBackgroundObject* obj)
{
	for (int i = 0; i < backgroundVector.size(); i++)
	{
		if (backgroundVector[i] == obj)
		{
			delete obj;
			backgroundVector.erase(backgroundVector.begin() + i);
			break;
		}
	}
}


void CVectorManager::DeleteObject(CEffect* effect)
{
	int effectAmount = effectVector.size();
	for (int c = 0; c < effectAmount; c++)
	{
		if (effectVector.at(c)->effectId == effect->effectId && effectVector.at(c)->obj == effect->obj)
		{
			delete effectVector.at(c);
			effectVector.erase(effectVector.begin() + c);
			c--;
			effectAmount--;
		}
	}
}

void CVectorManager::AddObject(CCommandObject* command)
{
	bool commandAlreadyExists = false;
	for (int i = 0; i < commandVector.size(); i++)
	{
		if (command->obj == commandVector.at(i)->obj && command->commandId == commandVector.at(i)->commandId)
		{
			commandAlreadyExists = true;
			break;
		}
	}
	if (command->commandId == command->stop || command->commandId == command->softstop)
	{
		CMoveCommand left(command->obj, 0);
		CMoveCommand right(command->obj, 1);
		DeleteObject(&left);
		DeleteObject(&right);
		CSlowMoveCommand slowleft(command->obj, 0);
		CSlowMoveCommand slowright(command->obj, 1);
		DeleteObject(&slowleft);
		DeleteObject(&slowright);
	}
	if (!commandAlreadyExists)
	{
		commandVector.push_back(command);
	}
	else delete command;
}

void CVectorManager::DeleteObject(CCommandObject* command)
{
	int commandAmount = commandVector.size();
	for (int c = 0; c < commandAmount; c++)
	{
		if (commandVector.at(c)->commandId == command->commandId && commandVector.at(c)->obj == command->obj)
		{
			delete commandVector.at(c);
			commandVector.erase(commandVector.begin() + c);
			c--;
			commandAmount--;
		}
	}
}
void CVectorManager::AddObject(CCrate* obj)
{
	baseVector.push_back(obj);
	solidVector.push_back(obj);
	crateVector.push_back(obj);
}
void CVectorManager::AddObject(CExplosion* obj)
{
	explosionVector.push_back(obj);
	baseVector.push_back(obj);
}
void CVectorManager::AddObject(CProjectile* obj)
{
	projectileVector.push_back(obj);
	baseVector.push_back(obj);
	physicsVector.push_back(obj);
}
void CVectorManager::AddObject(CPlayer* obj)
{
	animateVector.push_back(obj);
	player = obj;
	physicsVector.push_back(obj);
	baseVector.push_back(obj);
	solidVector.push_back(obj);
}
void CVectorManager::AddObject(CEnemy* obj)
{
	enemyVector.push_back(obj);
	animateVector.push_back(obj);
	baseVector.push_back(obj);
	physicsVector.push_back(obj);
	solidVector.push_back(obj);
}
void CVectorManager::AddObject(CTrapObject* obj)
{
	baseVector.push_back(obj);
	trapVector.push_back(obj);
}

void CVectorManager::AddObject(CWeapon* obj)
{
	weaponVector.push_back(obj);
}


void CVectorManager::AddObject(CItem* obj)
{
	baseVector.push_back(obj);
	physicsVector.push_back(obj);
	itemVector.push_back(obj);
}

void CVectorManager::AddObject(CChest* obj)
{
	baseVector.push_back(obj);
	chestVector.push_back(obj);
}

void CVectorManager::AddObject(CDamageNumber* obj)
{
	damageNumberVector.push_back(obj);
}


void CVectorManager::DeleteObjectsConcerning(CBaseObject * obj)
{
	for (int c = 0; c < commandVector.size(); c++)
	{
		if (commandVector.at(c)->obj == obj)
		{
			delete commandVector.at(c);
			commandVector.erase(commandVector.begin() + c);
			c--;
		}
	}

	for (int c = 0; c < effectVector.size(); c++)
	{
		if (effectVector.at(c)->obj == obj)
		{
			delete effectVector.at(c);
			effectVector.erase(effectVector.begin() + c);
			c--;
		}
	}
}

void CVectorManager::AddTile(CTile* tile)
{
	tRow.push_back(tile);
}
void CVectorManager::NewTileRow()
{
	tileVector.push_back(tRow);
	tRow.clear();
}
vector<CExplosion*>* CVectorManager::GetExplosionVector()
{
	return &explosionVector;
}

vector < vector<CTile*> >* CVectorManager::GetTileVector()
{
	return &tileVector;
}

vector<CChest*>* CVectorManager::GetChestVector()
{
	return &chestVector;
}

vector <CPhysicsObject*>* CVectorManager::GetPhysicsVector()
{
	return &physicsVector;
}
vector <CBaseObject*>* CVectorManager::GetBaseVector()
{
	return &baseVector;
}

vector <CProjectile*>* CVectorManager::GetProjectileVector()
{
	return &projectileVector;
}
vector <CWeapon*>* CVectorManager::GetWeaponVector()
{
	return &weaponVector;
}

vector <CBaseObject*>* CVectorManager::GetSolidVector()
{
	return &solidVector;
}

vector <CAnimate*>* CVectorManager::GetAnimateVector()
{
	return &animateVector;
}
vector <CEnemy*>* CVectorManager::GetEnemyVector()
{
	return &enemyVector;
}

vector <CTrapObject*>* CVectorManager::GetTrapVector()
{
	return &trapVector;
}

vector <CCommandObject*>* CVectorManager::GetCommandVector()
{
	return &commandVector;
}
vector <CEffect*>* CVectorManager::GetEffectVector()
{
	return &effectVector;
}
vector <CCrate*>* CVectorManager::GetCrateVector()
{
	return &crateVector;
}
vector<CItem*>* CVectorManager::GetItemVector()
{
	return &itemVector;
}

vector<CDamageNumber*>* CVectorManager::GetDamageNumberVector()
{
	return &damageNumberVector;
}

vector<CSpeech*>* CVectorManager::GetSpeechVector()
{
	return &speechVector;
}

SDL_Renderer * CVectorManager::GetRenderer()
{
	return renderer;
}
void CVectorManager::SetRenderPointer(SDL_Renderer* r)
{
	renderer = r;
}
void CVectorManager::AddObject(CSpeech*s)
{
		speechVector.push_back(s);
}