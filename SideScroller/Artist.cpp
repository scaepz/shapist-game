#include "Artist.h"
#include "Global.h"
#include "Line.h"
int playerPosOnScreen[2];
CArtist::CArtist(CVectorManager* _vm, int _tileSet, int resX, int resY, string textureDataFilePath, CDialogHandler * dh, SDL_Renderer * _renderer, SDL_Window * _window)
{
	fadeTimeInMs = 2000;
	dialogHandler = dh;
	vm = _vm;
	resolution[0] = resX;
	resolution[1] = resY;
	window = _window;
	renderer = _renderer;
	tileSet = _tileSet;
	tileVector = vm->GetTileVector();
	baseVector = vm->GetBaseVector();

	animateVector = vm->GetAnimateVector();
	weaponVector = vm->GetWeaponVector();

	hud = new CHud(renderer, vm, resolution[0], resolution[1]);

	levelTexture = IMG_LoadTexture(renderer, "res/tilesets/test.png");
	if (levelTexture == NULL)
	{
		std::cout << IMG_GetError() << std::endl;
		//throw exception
	}

	standardTileSize = vm->standardTileSize;
	sizeMultiplier = 1; //could be used for zoom, but not as things are right now
	player = vm->GetPlayer();

	//////////////
	//Load object textures
	std::fstream reader("res/textures/standardTextures.txt");
	string textureData;
	/*SDL_ShowCursor(0);
	SDL_Surface * tempSurface = SDL_LoadBMP("res/misc/crosshair.bmp");
	SDL_Cursor * cursor = SDL_CreateColorCursor(tempSurface, 12, 12);

	SDL_FreeSurface(tempSurface);
	SDL_SetCursor(cursor);
	SDL_ShowCursor(1);*/
	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		textureData += temp;
	}
	reader.close();
	//Load standard textures (weapons, projectiles etc)
	for (int i = 0; GetRow(textureData, i) != "eof"; i += 2)
	{
		textureMap[atoi(GetRow(textureData, i).c_str())] = IMG_LoadTexture(renderer, GetRow(textureData, i + 1).c_str());
		//textureVector.push_back(IMG_LoadTexture(renderer, GetRow(textureData, baseVector->at(i)->textureId).c_str()));
		std::cout << IMG_GetError() + i;
	}
	textureData = "";
	reader.open(textureDataFilePath);
	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		textureData += temp;
	}
	reader.close();
	//Load level specific textures
	for (int i = 0; GetRow(textureData, i) != "eof"; i += 2)
	{
		textureMap[atoi(GetRow(textureData, i).c_str())] = IMG_LoadTexture(renderer, GetRow(textureData, i + 1).c_str());
		//textureVector.push_back(IMG_LoadTexture(renderer, GetRow(textureData, baseVector->at(i)->textureId).c_str()));
		std::cout << IMG_GetError() + i;
	}

	inventory = IMG_LoadTexture(renderer, "res/inventory_slot.png");
	highlightedSlot = IMG_LoadTexture(renderer, "res/inventory_highlightedslot.png");
	currentSlot = IMG_LoadTexture(renderer, "res/inventory_currentslot.png");

	chestDialog = IMG_LoadTexture(renderer, "res/chestdialog_purple_window.png");
	chestDialogCloseButton = IMG_LoadTexture(renderer, "res/chestdialog_purple_close.png");
	FadeIn(2000);
}

void CArtist::DrawParticles()
{
	CParticleEngine * pe = vm->GetParticleEngine();
	pe->Update();

	//draw lines

	for (int i = 0; i < pe->lineVector.size(); i++)
	{
		CLine * line = pe->lineVector.at(i);
		SDL_SetRenderDrawColor(renderer, line->r, line->g, line->b, line->alpha);
		SDL_RenderDrawLine(renderer, line->x1 - screenPos[0], line->y1 - screenPos[1], line->x2 - screenPos[0], line->y2 - screenPos[1]);
	}

}


void CArtist::DrawHud()
{
	hud->Update();
	//damage numbers
	SDL_Texture * tempdmg = nullptr;
	SDL_Rect dest;
	int alpha = 0;
	while (hud->GetNextDamageNumber(tempdmg, dest, alpha))
	{
		dest.x -= screenPos[0];
		dest.y -= screenPos[1];
		SDL_SetTextureAlphaMod(tempdmg, alpha);
		SDL_RenderCopy(renderer, tempdmg, NULL, &dest);
	}

	//stamina

	SDL_SetRenderDrawColor(renderer, 255, 255 - hud->GetStaminaRedness(), 255 - hud->GetStaminaRedness(), 255); //set color to white (or red if stamina bar is flashing)

	SDL_RenderDrawRect(renderer, &hud->GetStaminaInnerBorderRect());

	SDL_RenderDrawRect(renderer, &hud->GetStaminaOuterBorderRect());

	SDL_RenderFillRect(renderer, &hud->GetStaminaBarRect());

	//hp

	SDL_RenderCopy(renderer, hud->GetHpTexture(), NULL, &hud->GetHpRect());

	//ammo
	SDL_RenderCopy(renderer, hud->GetClipTexture(), NULL, &hud->GetClipRect());
	SDL_RenderCopy(renderer, hud->GetReserveAmmoTexture(), NULL, &hud->GetReserveAmmoRect());

}
void CArtist::DrawDialogs()
{

	//draw inventory
	float invPercent = dialogHandler->GetInventory()->GetInventoryVerticalPositionPercent();
	if (invPercent > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (player->GetCurrentSlot() == i)
			{
				SDL_RenderCopy(renderer, currentSlot, NULL, &dialogHandler->GetSlotRect(i));
			}
			else if (dialogHandler->IsHighLighted(i))
			{
				SDL_RenderCopy(renderer, highlightedSlot, NULL, &dialogHandler->GetSlotRect(i));
			}
			else
			{
				SDL_RenderCopy(renderer, inventory, NULL, &dialogHandler->GetSlotRect(i));
			}

		}

		for (int i = 0; i < 4; i++)
		{
			SDL_RenderCopy(renderer, textureMap[weaponVector->at(player->WeaponAtSlot(i))->iconId], NULL, &dialogHandler->GetIconRect(i));

		}
	}
	//draw chestdialog
	if (dialogHandler->IsChestDialogOpen())
	{
		SDL_RenderCopy(renderer, chestDialog, NULL, &dialogHandler->GetChestRect());
		SDL_RenderCopy(renderer, textureMap[weaponVector->at(dialogHandler->chest->GetContent())->iconId], NULL, &dialogHandler->GetChestIconRect());
		SDL_RenderCopy(renderer, chestDialogCloseButton, NULL, &dialogHandler->GetChestCloseButtonRect());
	}
}
string CArtist::GetRow(const string & file, int rownumber)
{
	int rowCounter = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (rowCounter == rownumber)
		{
			int counter = 0;
			string row;
			while (true)
			{
				if (i + counter < file.size())
				{
					if (file.at(i + counter) != '\n')
					{
						counter++;
					}
					else
					{
						row = file.substr(i, counter);
						return row;
					}
				}
				else return "eof";
			}
		}
		if (file.at(i) == '\n')
		{
			rowCounter++;
		}
	}
	return "eof";
}
void CArtist::DrawEverything()
{

	CalculateScreenPos();
	SDL_RenderClear(renderer);

	background bg;
	bg.distanceMultiplier = 0.86;
	bg.height = 100;
	bg.width = 100;
	bg.textureId = 1000;

	DrawBackground(bg);
	DrawObjects();
	DrawParticles();
	DrawTiles();
	DrawSpeech();
	DrawHud();
	DrawDialogs();
	DrawFade();

	SDL_RenderPresent(renderer);
}
void CArtist::CalculateScreenPos()
{
	//calculate where the screen is, or what part of the level should be drawn on the screen
	screenPos[0] = player->pos[0] + ((player->GetWidth()*sizeMultiplier) / 2) - (resolution[0] / 2);
	screenPos[1] = player->pos[1] + ((player->GetHeight()*sizeMultiplier) / 2) - (resolution[1] / 2);
	//if screenpos is higher than size of level, set screenpos to size of level
	if (screenPos[0] + resolution[0] > (tileVector->at(0).size()) *standardTileSize)
	{
		screenPos[0] = (tileVector->at(0).size()) *standardTileSize - resolution[0];
	}
	if (screenPos[1] + resolution[1] > (tileVector->size()) *standardTileSize)
	{
		screenPos[1] = (tileVector->size()) *standardTileSize - resolution[1];
	}

	//if screeenpos is below 0, set screenpos to 0
	if (screenPos[0] < 0) screenPos[0] = 0;
	if (screenPos[1] < 0) screenPos[1] = 0;

	//playerPosOnScreen is the player position within [0 - resolution[0]] and [0-resolution[1]]. It is used to for mouse input (what direction should player be facing)
	playerPosOnScreen[0] = player->pos[0] + ((player->GetWidth()*sizeMultiplier) / 2) - screenPos[0];
	playerPosOnScreen[1] = player->pos[1] + ((player->GetHeight()*sizeMultiplier) / 2) - screenPos[1];

}

void CArtist::DrawSpeech()
{
	vector<CSpeech*> * sv = vm->GetSpeechVector();
	for (int i = 0; i < sv->size(); i++)
	{
		sv->at(i)->dst.x -= screenPos[0];
		sv->at(i)->dst.y -= screenPos[1];
		SDL_RenderCopy(renderer, sv->at(i)->texture, NULL, &sv->at(i)->dst);
	}
}

void CArtist::DrawObjects()
{

	bool renderedWeapons = false;

	for (int i = 0; i < vm->GetBackgroundVector()->size(); i++)
	{
		CBackgroundObject * obj = vm->GetBackgroundVector()->at(i);
		int tempX, tempY;
		if (obj->GetAdjustedX(resolution[0], screenPos[0], tempX) && obj->GetAdjustedY(resolution[1], screenPos[1], tempY))
		{
			SDL_Rect dst;
			dst.w = obj->GetWidth();
			dst.h = obj->GetHeight();
			dst.x = tempX;
			dst.y = tempY;
			SDL_RenderCopy(renderer, textureMap[obj->textureId], NULL, &dst);
		}

	}
	for (int renderLap = 0; renderLap < 5; renderLap++) //each object has a renderlap between 0 and 5. If a objects renderlap is 0 then it is rendered first and will thus be behind stuff rendered later
	{
		for (int i = 0; i < baseVector->size(); i++)
		{
			if (renderLap == 3 && !renderedWeapons)
			{
				for (int a = 0; a < animateVector->size(); a++)
				{
					renderedWeapons = true;
					if (animateVector->at(a)->GetWeaponEquipped() >= 0)
					{
						CWeapon * weapon = weaponVector->at(animateVector->at(a)->GetWeaponEquipped());



						SDL_Rect dstrect;

						dstrect.h = weapon->height;
						dstrect.w = weapon->width;

						int kickBackFlipper;
						SDL_RendererFlip flipper;
						if (animateVector->at(a)->lookingLeft)
						{
							flipper = SDL_FLIP_VERTICAL;
							kickBackFlipper = -1;
						}
						else
						{
							flipper = SDL_FLIP_NONE;
							kickBackFlipper = 1;
						}
						if (weapon->GetFrameSizeX() == 0)
						{
							if (animateVector->at(a)->currentAnim > 1) //if obj is not idle
							{

								dstrect.x = animateVector->at(a)->pos[0] + animateVector->at(a)->kickback->GetX()*kickBackFlipper + (animateVector->at(a)->GetWidth() / 2) - screenPos[0] + weapon->positionAdjustment[0];
								dstrect.y = animateVector->at(a)->pos[1] + animateVector->at(a)->kickback->GetY()*kickBackFlipper + (animateVector->at(a)->GetHeight() / 2) - screenPos[1] + weapon->positionAdjustment[1];
								//if (flipper == SDL_FLIP_VERTICAL) dstrect.y -= weapon->height / 8;
								SDL_RenderCopyEx(renderer,
									textureMap[weapon->textureId],
									NULL,
									&dstrect,
									animateVector->at(a)->GetXhairAngleDeg() + animateVector->at(a)->kickback->GetRotation()*kickBackFlipper,
									&weapon->flipPoint,
									flipper);
							}
							else
							{
								dstrect.x = animateVector->at(a)->pos[0] - screenPos[0] + weapon->idlePositionAdjustment[0];
								dstrect.y = animateVector->at(a)->pos[1] + (animateVector->at(a)->GetHeight() / 3) + weapon->idlePositionAdjustment[1] - screenPos[1];
								SDL_RenderCopyEx(renderer,
									textureMap[weapon->textureId],
									NULL,
									&dstrect,
									weapon->idleRotation,
									&weapon->flipPoint,
									SDL_FLIP_NONE);
							}
						}
						else
						{
							SDL_Rect srcrect;
							srcrect.h = weapon->GetFrameSizeY();
							srcrect.w = weapon->GetFrameSizeX();
							srcrect.x = (animateVector->at(a)->GetWeaponFrame() % 4) * srcrect.w;
							srcrect.y = (animateVector->at(a)->GetWeaponFrame() / 4) * srcrect.h;


							if (animateVector->at(a)->currentAnim > 1)
							{
								dstrect.x = animateVector->at(a)->pos[0] + animateVector->at(a)->kickback->GetX()*kickBackFlipper + (animateVector->at(a)->GetWidth() / 2) - screenPos[0] + weapon->positionAdjustment[0];
								dstrect.y = animateVector->at(a)->pos[1] + animateVector->at(a)->kickback->GetY()*kickBackFlipper + (animateVector->at(a)->GetHeight() / 2) - screenPos[1] + weapon->positionAdjustment[1];
								//std::cout << "h: " << dstrect.h << " w: " << dstrect.w << " x: " << dstrect.x << " y " << dstrect.y << std::endl;
								//if (flipper == SDL_FLIP_VERTICAL) dstrect.y -= weapon->height / 8;
								SDL_RenderCopyEx(renderer,
									textureMap[weapon->textureId],
									&srcrect,
									&dstrect,
									animateVector->at(a)->GetXhairAngleDeg() + animateVector->at(a)->kickback->GetRotation()*kickBackFlipper,
									&weapon->flipPoint,
									flipper);
							}
							else
							{
								dstrect.x = animateVector->at(a)->pos[0] - screenPos[0] + weapon->idlePositionAdjustment[0];
								dstrect.y = animateVector->at(a)->pos[1] + (animateVector->at(a)->GetHeight() / 3) + weapon->idlePositionAdjustment[1] - screenPos[1];
								SDL_RenderCopyEx(renderer,
									textureMap[weapon->textureId],
									&srcrect,
									&dstrect,
									weapon->idleRotation,
									&weapon->flipPoint,
									SDL_FLIP_NONE);
							}
						}
					}
				}
			}

			if (baseVector->at(i)->renderOrder == renderLap)
			{
				//if object is within bounds of screen 
				if (baseVector->at(i)->pos[0] + baseVector->at(i)->GetWidth() >= screenPos[0] &&
					baseVector->at(i)->pos[0] < screenPos[0] + resolution[0] &&
					baseVector->at(i)->pos[1] + baseVector->at(i)->GetHeight() >= screenPos[1] &&
					baseVector->at(i)->pos[1] < screenPos[1] + resolution[1])
				{

					SDL_Rect srcrect;
					srcrect.h = baseVector->at(i)->frameSizeY;
					srcrect.w = baseVector->at(i)->frameSizeX;
					srcrect.x = ((baseVector->at(i)->GetFrame() % 4)) * srcrect.w;
					srcrect.y = ((baseVector->at(i)->GetFrame() / 4)) * srcrect.h;

					SDL_Rect dstrect;
					dstrect.h = baseVector->at(i)->GetHeight() * sizeMultiplier;
					dstrect.w = baseVector->at(i)->GetWidth() * sizeMultiplier;;
					//put it at its on screen position
					dstrect.x = baseVector->at(i)->pos[0] - screenPos[0];
					dstrect.y = baseVector->at(i)->pos[1] - screenPos[1];
					//	std::cout << baseVector->at(i)->GetRotation();

					SDL_RendererFlip flipper;
					if (baseVector->at(i)->lookingLeft)
					{
						flipper = SDL_FLIP_HORIZONTAL;
					}
					else
					{
						flipper = SDL_FLIP_NONE;
					}
					SDL_RenderCopyEx(renderer, textureMap[baseVector->at(i)->textureId], &srcrect, &dstrect, baseVector->at(i)->GetRotation(), NULL, flipper);
				}
			}




		}
	}
}
void CArtist::DrawTiles()
{
	for (int row = screenPos[1] / standardTileSize; row < (screenPos[1] + resolution[1]) / standardTileSize + 1; row++)
	{
		if (row == tileVector->size())
		{
			break;
		}
		{
			for (int column = screenPos[0] / standardTileSize; column < (screenPos[0] + resolution[0]) / standardTileSize + 1; column++)
			{
				if (column == tileVector->at(row).size())
				{
					break;
				}
				CTile * tile = tileVector->at(row).at(column);
				if (tile != nullptr)
				{
					SDL_Rect srcrect;
					srcrect.h = 64;
					srcrect.w = 64;
					srcrect.x = ((tile->texture % 4)) * 64;
					srcrect.y = (tile->texture / 4) * 64;

					SDL_Rect dstrect;
					dstrect.h = sizeMultiplier*standardTileSize;
					dstrect.w = dstrect.h;
					dstrect.x = (sizeMultiplier * standardTileSize * tile->pos[0]) - screenPos[0];
					dstrect.y = (sizeMultiplier * standardTileSize * tile->pos[1]) - screenPos[1];
					SDL_RenderCopy(renderer, levelTexture, &srcrect, &dstrect);
				}
			}
		}
	}
}

void CArtist::DrawFade()
{
	if (fading)
	{
		if (fadeTimePassed > fadeTimeInMs)
		{
			fading = false;
			fadeTimePassed = 0;
		}
		else if (fadeDirectionIn)
		{
			SDL_Rect wholeScreenRect;
			wholeScreenRect.x = 0;
			wholeScreenRect.y = 0;
			wholeScreenRect.w = resolution[0];
			wholeScreenRect.h = resolution[1];

			blackness = 255 - (255.0f * ((float)((float)fadeTimePassed / (float)fadeTimeInMs))*(float)((float)fadeTimePassed / (float)fadeTimeInMs));


			SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackness);
			SDL_RenderFillRect(renderer, &wholeScreenRect);
			fadeTimePassed += g_time;
		}
		else
		{
			SDL_Rect wholeScreenRect;
			wholeScreenRect.x = 0;
			wholeScreenRect.y = 0;
			wholeScreenRect.w = resolution[0];
			wholeScreenRect.h = resolution[1];

			blackness = 255.0f * (float)((float)fadeTimePassed / (float)fadeTimeInMs);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackness);
			SDL_RenderFillRect(renderer, &wholeScreenRect);
			fadeTimePassed += g_time;
		}
	}
}

void CArtist::FadeIn(int ms)
{
	fading = true;
	fadeDirectionIn = true;
	fadeTimePassed = 0;
	fadeTimeInMs = ms;
}
void CArtist::FadeOut(int ms)
{
	fading = true;
	fadeDirectionIn = false;
	fadeTimePassed = 0;
	fadeTimeInMs = ms;
}

CArtist::~CArtist()
{
	//release all textures
	for (std::map<int, SDL_Texture*>::iterator i = textureMap.begin(); i != textureMap.end(); ++i)
	{
		SDL_DestroyTexture(i->second);
		i->second = nullptr;

	}
	SDL_DestroyTexture(levelTexture);


	TTF_CloseFont(speechFont);
	SDL_FreeSurface(message);
	SDL_DestroyTexture(messageT);
	SDL_DestroyTexture(inventory);
	SDL_DestroyTexture(grenadeIcon);
	SDL_DestroyTexture(lightningIcon);
	SDL_DestroyTexture(currentSlot);
	SDL_DestroyTexture(highlightedSlot);
	SDL_DestroyTexture(chestDialog);
	SDL_DestroyTexture(chestDialogCloseButton);


	delete hud;
}

void CArtist::DrawBackground(background bg)
{
	SDL_Rect dest;
	dest.w = bg.width;
	dest.h = bg.height;
	//for x = blabla - 2*bgwidth  <--- the 2 is a bandaid fix and might limit distancemultiplier range
	for (float x = (screenPos[0] % bg.width) - 3 * bg.width; x <= resolution[0] / bg.distanceMultiplier; x += bg.width)
	{
		dest.x = (resolution[0] - x) * bg.distanceMultiplier;
		for (float y = (screenPos[1] % bg.height) - 3 * bg.height; y <= resolution[0] / bg.distanceMultiplier; y += bg.height)
		{
			dest.y = (resolution[1] - y) * bg.distanceMultiplier;
			if (dest.x <= resolution[0] && dest.y <= resolution[1])
				SDL_RenderCopy(renderer, textureMap[bg.textureId], NULL, &dest);
		}
	}
}