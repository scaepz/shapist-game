#include "ProjectileHandler.h"
#include "SoundEnum.h"
ProjectileHandler::ProjectileHandler(CVectorManager* _vm)
{
	vm = _vm;
	baseVector = vm->GetBaseVector();
	projectileVector = vm->GetProjectileVector();
	objectVector = vm->GetPhysicsVector();
	explosionVector = vm->GetExplosionVector();

}


ProjectileHandler::~ProjectileHandler()
{
}
void ProjectileHandler::DoImpulse(CExplosion* & exp)
{
	//todo: get a brilliant idea and rewrite all this 
	for (int i = 0; i < objectVector->size(); i++)
	{
		CPhysicsObject* obj = objectVector->at(i);
		int ySurface;

		if (obj->pos[1] + obj->GetHeight() < exp->impulseCentre[1])
		{
			//object is above impulse center, act on bottom surface
			ySurface = 0;
		}
		else if (obj->pos[1] > exp->impulseCentre[1])
		{
			//object is below impulse center, act on top surface
			ySurface = 1;
		}
		else
		{
			ySurface = 2;
			// object surfaces are on both sides of impulse, act on centre.
		}
		int distanceX;
		int distanceY;
		float distanceMultiplier;
		if (obj->pos[0] + obj->GetWidth() < exp->impulseCentre[0])
		{

			//obj is left of impulse center
			switch (ySurface)
			{
			case 0:
				//object is above impulse center, act on bottom surface
				distanceX = obj->pos[0] + obj->GetWidth() - exp->impulseCentre[0];
				if (distanceX > -1 * exp->radius)
				{
					distanceY = obj->pos[1] - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (float)exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			case 1:
				//object is below impulse center, act on top surface
				distanceX = obj->pos[0] + obj->GetWidth() - exp->impulseCentre[0];
				if (distanceX > -1 * exp->radius)
				{
					distanceY = obj->pos[1] - exp->impulseCentre[1];
					if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (float)exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, (exp->impulse - (float)exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			case 2:
				// object surfaces are on both sides of impulse, act on centre.
				distanceX = obj->pos[0] + obj->GetWidth() - exp->impulseCentre[0];
				if (distanceX > -1 * exp->radius)
				{
					distanceY = obj->pos[1] + (obj->GetHeight() / 2) - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (float)exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
					else if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (float)exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, exp->impulse - (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			}
		}
		else if (obj->pos[0] > exp->impulseCentre[0])
		{//obj is right of impulse center
			switch (ySurface)
			{
			case 0:
				//object is above impulse center, act on bottom surface
				distanceX = obj->pos[0] - exp->impulseCentre[0];
				if (distanceX < exp->radius)
				{
					distanceY = obj->pos[1] + obj->GetHeight() - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			case 1:
				//object is below impulse center, act on top surface
				distanceX = obj->pos[0] - exp->impulseCentre[0];
				if (distanceX < exp->radius)
				{
					distanceY = obj->pos[1] - exp->impulseCentre[1];
					if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, exp->impulse - (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			case 2:
				// object surfaces are on both sides of impulse, act on centre.
				distanceX = obj->pos[0] - exp->impulseCentre[0];
				if (distanceX < exp->radius)
				{
					distanceY = obj->pos[1] + (obj->GetHeight() / 2) - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
					else if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			}
		}
		else
		{// object X-surfaces are on both sides of impulse, act on centre.
			switch (ySurface)
			{
			case 0:
				//object is above impulse center, act on bottom surface
				distanceX = obj->pos[0] + (obj->GetWidth() / 2) - exp->impulseCentre[0];
				if (distanceX < exp->radius && distanceX >= 0)
				{
					distanceY = obj->pos[1] + obj->GetHeight() - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				else if (distanceX > -1 * exp->radius && distanceX <= 0)
				{
					distanceY = obj->pos[1] + obj->GetHeight() - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			case 1:
				//object is below impulse center, act on top surface
				distanceX = obj->pos[0] + (obj->GetWidth() / 2) - exp->impulseCentre[0];
				if (distanceX < exp->radius && distanceX >= 0)
				{
					distanceY = obj->pos[1] - exp->impulseCentre[1];
					if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, exp->impulse - (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				else if (distanceX > -1 * exp->radius && distanceX <= 0)
				{
					distanceY = obj->pos[1] - exp->impulseCentre[1];
					if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier), exp->impulse - (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}

				break;
			case 2:
				// object surfaces are on both sides of impulse, act on centre.
				distanceX = obj->pos[0] + (obj->GetWidth() / 2) - exp->impulseCentre[0];
				if (distanceX < exp->radius && distanceX >= 0)
				{
					distanceY = obj->pos[1] + (obj->GetHeight() / 2) - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
					else if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(exp->impulse / (1 + 1 * abs(multiplier))*distanceMultiplier, (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}

				}
				else if (distanceX > -1 * exp->radius && distanceX <= 0)
				{
					distanceY = obj->pos[1] + (obj->GetHeight() / 2) - exp->impulseCentre[1];
					if (distanceY > -1 * exp->radius && distanceY <= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier, -1 * (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
					else if (distanceY < exp->radius && distanceY >= 0)
					{
						distanceMultiplier = sqrt(distanceX*distanceX + distanceY*distanceY);
						distanceMultiplier /= exp->radius;
						distanceMultiplier = 1 - distanceMultiplier;
						distanceMultiplier /= obj->weight;
						double multiplier = (float)(distanceY) / (float)(distanceX);
						CVector2f imp(-1 * (exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier, (exp->impulse - exp->impulse / (1 + 1 * abs(multiplier)))*distanceMultiplier);
						obj->AddVelocity(imp);
					}
				}
				break;
			}
		}
	}
}
void ProjectileHandler::HandleProjectiles()
{

	for (int i = 0; i < projectileVector->size(); i++)
	{
		if (!projectileVector->at(i)->UpdateLife())
		{
			if (projectileVector->at(i)->explosionData[0] == 1)
			{


				//create explosion object with stats from projectile
				CExplosion* explosion = new CExplosion(projectileVector->at(i)->explosionData[1], projectileVector->at(i)->explosionData[2], projectileVector->at(i)->explosionData[3],
					projectileVector->at(i)->pos[0] + (projectileVector->at(i)->GetWidth() / 2) - (projectileVector->at(i)->explosionData[1] / 2)
					, projectileVector->at(i)->pos[1] + (projectileVector->at(i)->GetHeight() / 2) - (projectileVector->at(i)->explosionData[1] / 2), 501);
				vm->GetSoundPlayer()->PlaySound(explosion_small, 0);

				//check if any solids (aka damagables) are within range
				for (int x = 0; x < vm->GetSolidVector()->size(); x++)
				{
					CBaseObject * obj = vm->GetSolidVector()->at(x);

					//calculate distance between proj and obj
					int distance;
					distance = sqrt((projectileVector->at(i)->GetX() - (obj->GetX() + obj->GetWidth() / 2))*(projectileVector->at(i)->GetX() - (obj->GetX() + obj->GetWidth() / 2)) + (projectileVector->at(i)->GetY() - (obj->GetY() + obj->GetHeight() / 2)) * (projectileVector->at(i)->GetY() - (obj->GetY() + obj->GetHeight() / 2)));

					//if proj is almost right on the obj
					if (distance <= sqrt(obj->GetWidth() / 2 * obj->GetWidth() / 2 + obj->GetHeight() / 2 + obj->GetHeight() / 2) + 8) // 8 + size/2 is the magic distance within which a projectile deals full damage
					{
						//take full damage
						int damage = projectileVector->at(i)->explosionData[3];
						obj->TakeDamage(damage);

						//if the player fired the proj then show dmg numbers and play hit sound
						if (projectileVector->at(i)->firedByPlayer)
						{
							if (vm->GetPlayer() == obj) //if player damages himself
							{
								damage /= 3; //lessen the damage					
							}
							else
							{
								vm->GetSoundPlayer()->PlayDamageSound(damage);
							}
							CDamageNumber * dmgNumber = new CDamageNumber(damage, obj->GetX() + obj->GetWidth() / 2, obj->GetY());
							vm->AddObject(dmgNumber);
						}
					}
					//else if distance is lower than range
					else if (distance < projectileVector->at(i)->explosionData[1])
					{
						//damage = damage - damage * (distance / range); (linear fall off)
						int damage = projectileVector->at(i)->explosionData[3] - (projectileVector->at(i)->explosionData[3] * (distance / projectileVector->at(i)->explosionData[1]));

						obj->TakeDamage(damage);
						//if the player fired the proj then show dmg numbers and play hit sound
						if (projectileVector->at(i)->firedByPlayer)
						{
							if (vm->GetPlayer() == obj) //if player damages himself
							{
								damage /= 3; //lessen the damage
								//play hurt sound
							}
							else
							{
								vm->GetSoundPlayer()->PlayDamageSound(damage);
							}
							CDamageNumber * dmgNumber = new CDamageNumber(damage, obj->GetX() + obj->GetWidth() / 2, obj->GetY());
							vm->AddObject(dmgNumber);
						}
					}
				}
				vm->AddObject(explosion);
				vm->DeleteObject(projectileVector->at(i));
				DoImpulse(explosion);
				i--;

			}
		}
	}
	HandleExplosions();
}
void ProjectileHandler::HandleExplosions()
{
	for (int i = 0; i < explosionVector->size(); i++)
	{
		if (!explosionVector->at(i)->IsHealthy())
		{
			vm->DeleteObject(explosionVector->at(i));
			i--;
		}
	}
}


