#include "DeathManager.h"
#include "CollisionDetector.h"

CDeathManager::CDeathManager(CVectorManager * _vm, std::map<int, CGroup*> * _groupMap)
{
	groupMap = _groupMap;
	vm = _vm;
	animateVector = vm->GetAnimateVector();
	crateVector = vm->GetCrateVector();
}


CDeathManager::~CDeathManager()
{
}

void CDeathManager::UpdateCrates()
{
	for (int b = 0; b < crateVector->size(); b++)
	{
		if (!crateVector->at(b)->IsHealthy())
		{
			if (crateVector->at(b)->Die())
			{
				if (crateVector->at(b)->GetContent() != 0)
				{
					switch (crateVector->at(b)->GetContent())
					{
					case redHP:
					{
								  CHealthBubble* hp = new CHealthBubble(crateVector->at(b)->pos[0] + (crateVector->at(b)->GetWidth() / 2) - 16, crateVector->at(b)->pos[1], 401, 16, 25);
								  vm->AddObject(hp);
								  break;
					}
					case greenHP:
					{
									CHealthBubble* hp = new CHealthBubble(crateVector->at(b)->pos[0] + (crateVector->at(b)->GetWidth() / 2) - 16, crateVector->at(b)->pos[1], 402, 32, 50);
									vm->AddObject(hp);
									break;
					}
					case purpleKey:
					{
									  CKey* key = new CKey(crateVector->at(b)->pos[0] + (crateVector->at(b)->GetWidth() / 2) - 16, crateVector->at(b)->pos[1], 403, 2);
									  vm->AddObject(key);
									  break;
					}
					default:
						break;
					}
				}
				vm->DeleteObject(crateVector->at(b));
				b--;
			}
		}
	}
	for (int i = 0; i < vm->GetItemVector()->size(); i++)
	{
		CCollisionDetector cd;

		if (cd.Collides(vm->GetItemVector()->at(i), vm->GetPlayer()))
		{
			vm->GetItemVector()->at(i)->GetPickedUp(vm->GetPlayer());
			vm->DeleteObject(vm->GetItemVector()->at(i));
		}
	}
}

void CDeathManager::UpdateAnimates()
{
	for (int b = 0; b < animateVector->size(); b++)
	{
		if (!animateVector->at(b)->IsHealthy())
		{
			if (animateVector->at(b)->Die())
			{
				if (animateVector->at(b)->GetLoot() != 0)
				{
					switch (animateVector->at(b)->GetLoot())
					{
					case redHP:
					{
								  CHealthBubble* hp = new CHealthBubble(animateVector->at(b)->pos[0] + (animateVector->at(b)->GetWidth() / 2) - 8, animateVector->at(b)->pos[1], 2001, 16, 25);
								  vm->AddObject(hp);
								  break;
					}
					case greenHP:
					{
									CHealthBubble* hp = new CHealthBubble(animateVector->at(b)->pos[0] + (animateVector->at(b)->GetWidth() / 2) - 16, animateVector->at(b)->pos[1], 2002, 32, 50);
									vm->AddObject(hp);
									break;
					}
					case purpleKey:
					{
									  CKey* key = new CKey(animateVector->at(b)->pos[0] + (animateVector->at(b)->GetWidth() / 2) - 16, animateVector->at(b)->pos[1], 2003, 2);
									  vm->AddObject(key);
									  break;
					}
					default:
						break;
					}
				}
				if (groupMap->count(animateVector->at(b)->groupNumber == 1))
					groupMap->at(animateVector->at(b)->groupNumber)->RemoveMember(animateVector->at(b));
				vm->DeleteObject(animateVector->at(b));

				b--;
			}
		}
	}
	for (int i = 0; i < vm->GetItemVector()->size(); i++)
	{
		CCollisionDetector cd;
		if (!vm->GetPlayer()->IsDying())
		{
			if (cd.Collides(vm->GetItemVector()->at(i), vm->GetPlayer()))
			{
				vm->GetItemVector()->at(i)->GetPickedUp(vm->GetPlayer());
				vm->DeleteObject(vm->GetItemVector()->at(i));
			}
		}
	}
}