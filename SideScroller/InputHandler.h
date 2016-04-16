#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
//CommandFactory
#include <SDL.h>
#include <vector>
#include "Commands.h"
#include "VectorManager.h"
#include "Player.h"
#include "DialogHandler.h"

class CInputHandler
{
public:
	CInputHandler(CVectorManager * _vm, CDialogHandler * dialogHandler);
	virtual ~CInputHandler();
	void HandleInput();

private:
	enum Commands
	{
		moveLeft,
		moveRight,
		jump,
		stop,
		softStopLeft,
		softStopRight,
		dash,
		attack,
		slot0,
		slot1,
		nextWeapon,
		previousWeapon,
		openInventory, //not a commandobject
		interact,
		numberOfCommands
	};
	int a;
	int keyMap[numberOfCommands];
	CVectorManager* vm;

	std::vector<CCommandObject*>* commandVector;
	CPlayer * player;
	CDialogHandler * dialogHandler;
};

#endif