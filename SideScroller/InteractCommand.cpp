#include "InteractCommand.h"


CInteractCommand::CInteractCommand(CPlayer * _obj, vector<CChest*> * _chestVector, CDialogHandler * _dialogHandler) : CCommandObject(_obj)
{
	commandId = interact;
	chestVector = _chestVector;
	dialogHandler = _dialogHandler;
	player = _obj;
}


CInteractCommand::~CInteractCommand()
{
}

bool CInteractCommand::Execute()
{
	CCollisionDetector cd;
	for (int i = 0; i < chestVector->size(); i++)
	{
		if (cd.Collides(obj, chestVector->at(i)))
		{
			if (chestVector->at(i)->IsLocked())
			{
				if (player->HasKey(chestVector->at(i)->GetKey()))
				{
					player->RemoveKey(chestVector->at(i)->GetKey());
					chestVector->at(i)->Unlock();
					dialogHandler->OpenChestDialog(chestVector->at(i)); //also deletes all player commands and effects
				}
			}
			else
			{
				dialogHandler->OpenChestDialog(chestVector->at(i));
			}
		}
	}
	return true;
}
