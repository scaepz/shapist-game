#include "PhysicsHandler.h"
#include <iostream>

CPhysicsHandler::CPhysicsHandler(CVectorManager * _vm)
{
	vm = _vm;
	standardTileSize = vm->standardTileSize;
	objectVector = vm->GetPhysicsVector();
	tileVector = vm->GetTileVector();
	solidVector = vm->GetSolidVector();

	gravity = 0.00155;
	friction = 0.9995;


}


CPhysicsHandler::~CPhysicsHandler()
{
}

void CPhysicsHandler::ApplyGravity()
{
	for (int i = 0; i < objectVector->size(); i++)
	{
		if (objectVector->at(i)->OnGround() == false && objectVector->at(i)->IsSubjectToGravity())
		{
			//Apply gravity
			CVector2<double> grav(0, gravity*g_time);
			objectVector->at(i)->AddVelocity(grav);
		}
	}
}

void CPhysicsHandler::DoPhysics()
{
	ApplyGravity();
	UpdateObjectPositions();
	for (int i = 0; i < objectVector->size(); i++)
	{
		FixCollision(i);
	}
}
void CPhysicsHandler::UpdateObjectPositions()
{
	for (int i = 0; i < objectVector->size(); i++)
	{
		objectVector->at(i)->Update(g_time);
	}
}

void CPhysicsHandler::FixCollision(int index)
{
	//works by seeing if objects' new positions collide with other objects or tiles, and if they do moving them one step (1 ms) backwards on their trajectory (-1 * velocity)
	try
	{
		CCollisionDetector collisionDetector;
		enum dirToStopEnum
		{
			horizontal,
			vertical
		};
		bool stopDir[2] = { false, false };
		CPhysicsObject* obj = objectVector->at(index);


		bool collisionInThisLoop;
		do{
			collisionInThisLoop = false;
			for (double o = (obj->pos[1] / standardTileSize); o < (obj->pos[1] + obj->GetHeight()) / standardTileSize + 1; o++)
			{
				for (double i = (obj->pos[0] / standardTileSize); i <= ((obj->pos[0] + obj->GetWidth()) / standardTileSize) + 1; i++)
				{
					if (o < 0 || i < 0)
					{
						throw - 2;
					}
					else if (o > tileVector->size() || i > tileVector->at(o).size())
					{
						throw - 3;
					}
					CTile* tile = tileVector->at(o).at(i);
					{

						if (collisionDetector.Collides(obj, tile, standardTileSize))
						{

							bool collision = true;

							collisionInThisLoop = true;
							int counter = 0;
							while (collision)
							{
								if (counter > g_time + 1)
								{
									throw - 1;
								}
								counter += 2;
								obj->pos[0] -= 2 * obj->velocity[0];
								obj->pos[1] -= 2 * obj->velocity[1];

								if (!collisionDetector.Collides(obj, tile, standardTileSize))
								{
									if ((tile->pos[0] + 1) * standardTileSize < obj->pos[0])
									{
										stopDir[horizontal] = true;
									}
									else if ((tile->pos[0]) * standardTileSize > obj->pos[0] + obj->GetWidth())							
									{
										stopDir[horizontal] = true;
									}
									else if ((tile->pos[1])*standardTileSize + 1 < obj->pos[1])
									{
										stopDir[vertical] = true;
									}
									else if ((tile->pos[1]) * standardTileSize > obj->pos[1] + obj->GetHeight())
									{
										stopDir[vertical] = true;
									}
									collision = false;
								}
								if (stopDir[horizontal] && !stopDir[vertical])
								{
									obj->pos[1] += counter * obj->velocity[1];
								}
							}
						}
					}
				}
			}
		} while (collisionInThisLoop);


		bool standingOnEntity = false;

		//check for collisions with other physics objects
		/*	do{
				collisionInThisLoop = false;
				for (int i = 0; i < objectVector->size(); i++)
				{
				if (i != index)
				{

				CPhysicsObject* obj2 = objectVector->at(i);
				{
				if (collisionDetector.Collides(obj, obj2))
				{

				bool collision = true;

				collisionInThisLoop = true;

				int counter = 0;
				while (collision)
				{
				if (counter > g_time + 1)
				{
				throw i;
				}
				counter += 1;
				if (!obj2->smallObject)
				{
				obj->pos[0] -= obj->velocity[0];
				obj->pos[1] -= obj->velocity[1];
				}
				else
				{
				obj2->pos[0] -= obj2->velocity[0];
				obj2->pos[1] -= obj2->velocity[1];
				}
				//std::cout << "corrected";
				//	obj2->pos[0] -= obj2->velocity[0];
				//obj2->pos[1] -= obj2->velocity[1];

				if (!collisionDetector.Collides(obj, obj2))
				{

				if ((int)(obj->pos[0] >= (int)(obj2->pos[0]) + obj2->GetWidth()))
				{
				if (obj->velocity[0] > 0)
				{
				stopDir[horizontal] = true;
				if (obj2->smallObject)
				{
				obj2->OnCollision(true, false);
				}
				}

				}
				else if ((int)(obj->pos[0] + obj->GetWidth()) <= (int)(obj2->pos[0]))
				{
				if (obj->velocity[0] < 0)

				{
				stopDir[horizontal] = true;
				if (obj2->smallObject)
				{
				obj2->OnCollision(true, false);
				}
				}
				}
				else if ((int)(obj2->pos[1] + obj2->GetHeight()) < (int)(obj->pos[1]) && obj->velocity[1] < 0)
				{
				stopDir[vertical] = true;
				if (obj2->smallObject)
				{
				obj2->OnCollision(false, true);
				}
				}
				else if ((int)(obj2->pos[1]) > (int)(obj->pos[1] + obj->GetHeight()) && obj->velocity[1] > 0)
				{
				stopDir[vertical] = true;
				if (obj2->smallObject)
				{
				obj2->OnCollision(false, true);
				}
				else
				standingOnEntity = true;
				}


				collision = false;
				}
				}
				}//if object is on top of object2
				else if (obj->GetY() + obj->GetHeight() <= obj2->GetY() && obj->GetY() + obj->GetHeight() > obj2->GetY() - 1 && obj->pos[0] + obj->GetWidth() > obj2->pos[0] && obj2->pos[0] + obj2->GetWidth() > obj->pos[0])
				{
				standingOnEntity = true;
				}
				}
				}
				}

				} while (collisionInThisLoop);
				*/
		//check for collision with solid non-physics objects
		if (!objectVector->at(index)->ableToPassThroughEntities)
		{
			do{
				collisionInThisLoop = false;
				for (int i = 0; i < solidVector->size(); i++)
				{
					if (objectVector->at(index) != solidVector->at(i))
					{
						CBaseObject* obj2 = solidVector->at(i);
						{
							if (collisionDetector.Collides(obj, obj2))
							{
								bool collision = true;

								collisionInThisLoop = true;
								int counter = 0;
								while (collision)
								{
									if (counter > g_time + 1)
									{
										throw - 1;
									}
									counter += 1;
									obj->pos[0] -= obj->velocity[0];
									obj->pos[1] -= obj->velocity[1];

									if (!collisionDetector.Collides(obj, obj2))
									{
										if ((int)(obj->pos[0] >= (int)(obj2->pos[0]) + obj2->GetWidth()))
										{
											stopDir[horizontal] = true;


										}
										else if ((int)(obj->pos[0] + obj->GetWidth()) <= (int)(obj2->pos[0]))
										{
											stopDir[horizontal] = true;


										}
										else if ((int)(obj2->pos[1] + obj2->GetHeight()) <= (int)(obj->pos[1]) && obj->velocity[1] < 0)
										{
											stopDir[vertical] = true;


										}
										else if ((int)(obj2->pos[1]) > (int)(obj->pos[1] + obj->GetHeight()) && obj->velocity[1] > 0)
										{
											stopDir[vertical] = true;

											standingOnEntity = true;

										}
										collision = false;
									}
								}
							}//if object is on tof of object2
							else if (obj->GetY() + obj->GetHeight() <= obj2->GetY() && obj->GetY() + obj->GetHeight() > obj2->GetY() - 1 && obj->pos[0] + obj->GetWidth() > obj2->pos[0] && obj2->pos[0] + obj2->GetWidth() > obj->pos[0])
							{
						standingOnEntity = true;
							}

						}
					}
				}

			} while (collisionInThisLoop);
		}

		obj->OnCollision(stopDir[horizontal], stopDir[vertical]);

		obj->SetOnGround(false);

		//if object is standing on another object
		if (standingOnEntity)
		{
			//	std::cout << "standing ";
			obj->OnCollision(false, true);
			obj->SetOnGround(true);
			if (obj->velocity[1] > 0)
			{
				obj->velocity[1] = 0;
			}
			for (int fric = 0; fric < g_time; fric++)
			{
				obj->ApplyFriction(friction);
			}
		}
		else //if object is standing on a tile
		{
			//but first outside-of-map guard					TODO: doesn't currently work, if we spam impulse objects game will crash
			// but only when they get stuck inside of the player... some other issue might be causing it
			if ((obj->pos[1] + obj->GetHeight() + 1) / standardTileSize < 0
				|| (obj->pos[1] + obj->GetHeight() + 1) / standardTileSize >= tileVector->size()
				|| (obj->GetWidth() / standardTileSize) >= tileVector->size()
				|| obj->GetWidth() / standardTileSize < 0)
			{
				//save object from being launched outside map
				obj->pos[0] = obj->previousFramePos[0];
				obj->pos[1] = obj->previousFramePos[1];
				obj->velocity.SetX(0);
				obj->velocity.SetY(0);
			}
			else
			{
				for (int x = 0; x <= (obj->GetWidth()) / standardTileSize; x++)
				{
					if (obj->pos[0] >= 0 && obj->pos[0] / standardTileSize < tileVector->at(0).size() && obj->pos[1] >= 0 && obj->pos[1] / standardTileSize < tileVector->size())
					{
						if (tileVector->at((obj->pos[1] + obj->GetHeight() + 1) / standardTileSize).at((obj->pos[0]) / standardTileSize + x) != nullptr)
						{
							obj->SetOnGround(true);
							for (int fric = 0; fric < g_time; fric++)
							{
								obj->ApplyFriction(friction);
							}
						}
					}
				}
			}
		}
	}
	catch (int e)
	{
		//	std::cout << "throw" << index << std::endl;
		objectVector->at(index)->pos[0] = objectVector->at(index)->previousFramePos[0];
		objectVector->at(index)->pos[1] = objectVector->at(index)->previousFramePos[1];
		objectVector->at(index)->velocity[0] = 0;
		objectVector->at(index)->velocity[1] = 0;
		if (e > -1)
		{
			objectVector->at(e)->pos[0] = objectVector->at(e)->previousFramePos[0];
			objectVector->at(e)->pos[1] = objectVector->at(e)->previousFramePos[1];
			objectVector->at(e)->velocity[0] = 0;
			objectVector->at(e)->velocity[1] = 0;
		}
	}
}