#include "InputHandler.h"
#include <iostream>


CInputHandler::CInputHandler(CVectorManager * _vm, CDialogHandler * _dialogHandler)
{
	dialogHandler = _dialogHandler;
	vm = _vm;
	player = vm->GetPlayer();
	keyMap[jump] = SDLK_w;
	keyMap[dash] = SDLK_LSHIFT;
	keyMap[moveRight] = SDLK_d;
	keyMap[moveLeft] = SDLK_a;
	keyMap[stop] = SDLK_s;
	keyMap[attack] = SDL_BUTTON_LEFT;
	keyMap[slot0] = SDLK_1;
	keyMap[slot1] = SDLK_2;
	keyMap[nextWeapon] = SDL_MOUSEWHEEL;
	keyMap[previousWeapon] = SDL_MOUSEWHEEL;
	keyMap[openInventory] = SDLK_q;
	keyMap[interact] = SDLK_e;


	keyMap[softStopRight] = keyMap[moveRight];
	keyMap[softStopLeft] = keyMap[moveLeft];


}


CInputHandler::~CInputHandler()
{
}

void CInputHandler::HandleInput()
{
	if (!dialogHandler->IsDialogOpen())
	{
		int x1; int y1;
		SDL_GetMouseState(&x1, &y1);

		player->SetPointingDirection(x1 - playerPosOnScreen[0], y1 - playerPosOnScreen[1]); //name is slightly misleading //a month later im curious to know in what way

		player->SetRotation(90 - (atan2(x1 - playerPosOnScreen[0], y1 - playerPosOnScreen[1])*(180 / 3.1415)));
	}
	bool falseRightMoveKeyUpFix = false;
	bool falseLeftMoveKeyUpFix = false;


	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		if (dialogHandler->IsDialogOpen())
		{
			switch (ev.type)
			{
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == keyMap[interact])
				{
					if (dialogHandler->IsChestDialogOpen())
					{
						dialogHandler->CloseChestDialog();
					}					
				}
				break;
			case SDL_MOUSEBUTTONDOWN:

				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					dialogHandler->Click(ev.button.x, ev.button.y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button = SDL_BUTTON_LEFT)
				{
					dialogHandler->Release(ev.button.x, ev.button.y);
				}
				break;

			}
		}

		else
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.sym == keyMap[attack])
				{
					CAttackCommand *attack = new CAttackCommand(player);
					vm->AddObject(attack);
				}
				else if (ev.key.keysym.sym == keyMap[interact])
				{
					CCommandObject * interact = new CInteractCommand(player, vm->GetChestVector(), dialogHandler);
					vm->AddObject(interact);
				}
				else if (ev.key.keysym.sym == SDLK_r)
				{
					debugPause = true;
				}
				else if (ev.key.keysym.sym == keyMap[moveRight])

				{


					if (!ev.key.repeat)
					{

						CMoveCommand *move = new CMoveCommand(player, move->right);
						vm->AddObject(move);
						falseRightMoveKeyUpFix = true;
						CSoftStopCommand softStop(player);
						vm->DeleteObject(&softStop);
					}
				}
				else if (ev.key.keysym.sym == keyMap[moveLeft])
				{
					if (!ev.key.repeat)
					{
						CMoveCommand *move = new CMoveCommand(player, move->left);
						vm->AddObject(move);
						falseLeftMoveKeyUpFix = true;
						CSoftStopCommand softStop(player);
						vm->DeleteObject(&softStop);

					}
				}
				else if (ev.key.keysym.sym == keyMap[jump])
				{
					if (!ev.key.repeat)
					{


						CJumpCommand *jump = new CJumpCommand(player);
						vm->AddObject(jump);
					}
				}
				else if (ev.key.keysym.sym == keyMap[dash])
				{
					if (!ev.key.repeat){
						int x; int y;
						SDL_GetMouseState(&x, &y);

						CDashCommand *dash = new CDashCommand(player, x - playerPosOnScreen[0], y - playerPosOnScreen[1]);
						vm->AddObject(dash);
					}
				}
				else if (ev.key.keysym.sym == keyMap[stop])
				{
					if (!ev.key.repeat)
					{
						CStopCommand *stop = new CStopCommand(player);
						vm->AddObject(stop);
					}
				}
				else if (ev.key.keysym.sym == keyMap[slot0])
				{
					CSwitchWeapon *switchWeapon = new CSwitchWeapon(player, 0);
					vm->AddObject(switchWeapon);
				}
				else if (ev.key.keysym.sym == keyMap[slot1])
				{
					CSwitchWeapon *switchWeapon = new CSwitchWeapon(player, 1);
					vm->AddObject(switchWeapon);
				}
				else if (ev.key.keysym.sym == keyMap[openInventory])
				{

					dialogHandler->GetInventory()->OpenInventory();
				}
			}
			else if (ev.type == SDL_KEYUP)
			{
				if (ev.key.keysym.sym == keyMap[softStopRight])
				{
					if (!ev.key.repeat)
					{
						if (!falseRightMoveKeyUpFix)
						{
							CMoveCommand move(player, 1);

							vm->DeleteObject(&move);

							CSoftStopCommand *softstop = new CSoftStopCommand(player);
							vm->AddObject(softstop);
						}
					}
				}

				else if (ev.key.keysym.sym == keyMap[stop])
				{
					CStopCommand stop(player);
					vm->DeleteObject(&stop);
				}

				else if (ev.key.keysym.sym == keyMap[softStopLeft])
				{
					if (!ev.key.repeat)
					{
						if (!falseLeftMoveKeyUpFix)
						{


							CMoveCommand move(player, 0);
							vm->DeleteObject(&move);

							CSoftStopCommand *softstop = new CSoftStopCommand(player);
							vm->AddObject(softstop);
						}
					}
				}
				else if (ev.key.keysym.sym == keyMap[jump])
				{
					if (!ev.key.repeat)
					{
						CJumpCommand *jump = new CJumpCommand(player);
						vm->DeleteObject(jump);
					}
				}
				else if (ev.key.keysym.sym == keyMap[attack])
				{
					CAttackCommand attack(player);

					vm->DeleteObject(&attack);
					CCommandObject *ceaseFire = new CCeaseFire(player);
					vm->AddObject(ceaseFire);
				}
				else if (ev.key.keysym.sym == keyMap[openInventory])
				{
					dialogHandler->CloseInventory();
				}
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					if (dialogHandler->IsInventoryOpen())
					{
						dialogHandler->Click(ev.button.x, ev.button.y);
					}
				}
				if (ev.button.button == keyMap[attack])
				{
					if (!dialogHandler->IsInventoryOpen())
					{
						CAttackCommand *attack = new CAttackCommand(player);
						vm->AddObject(attack);
					}
				}

			}
			else if (ev.type == SDL_MOUSEBUTTONUP)
			{
				if (ev.button.button == keyMap[attack])
				{

					dialogHandler->Release(ev.button.x, ev.button.y);
					CAttackCommand attack(player);
					vm->DeleteObject(&attack);
					CCommandObject *ceaseFire = new CCeaseFire(player);
					vm->AddObject(ceaseFire);

				}
			}

		}
	}
}
