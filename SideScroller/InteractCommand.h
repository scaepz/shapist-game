#ifndef INTERACTCOMMAND_H
#define INTERACTCOMMAND_H

#include <vector>
#include "Chest.h"
#include "CollisionDetector.h"
#include "DialogHandler.h"
#include "CommandObject.h"
#include "Player.h"
class CInteractCommand :
	public CCommandObject
{
public:
	CInteractCommand(CPlayer * _obj, vector<CChest*> * chestVector, CDialogHandler *);
	virtual ~CInteractCommand();

	bool Execute();
private:
	//needed for chest interaction
	CDialogHandler * dialogHandler;
	vector<CChest*> * chestVector;
	CPlayer * player;
};


#endif // !INTERACTCOMMAND_H
