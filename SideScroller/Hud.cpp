#include "Hud.h"


CHud::CHud(SDL_Renderer * _renderer, CVectorManager * _vm, int resX, int resY)
{

	resolution[0] = resX;
	resolution[1] = resY;
	damageNumberIndex = 0;
	renderer = _renderer;
	vm = _vm;
	hpFont = TTF_OpenFont("res/fonts/Geomancy - Extra Bold.otf", 72);
	dmgNumberFont = TTF_OpenFont("res/fonts/F25_Executive.otf", 16);
	damageNumberPixels[0] = 10;
	damageNumberPixels[1] = 16;
	lastHp = -10;
	lastReserve = -10;
	lastClip = -10;
	
	//////
	//calculate stamina size and position
	staminaBarMaxWidth = resolution[0] / 5;
	staminaBarRect.w = staminaBarMaxWidth;
	staminaBarRect.h = resolution[1] / 30;
	staminaBarRect.x = resolution[0] / 2 - staminaBarRect.w / 2;
	staminaBarRect.y = resolution[1] - staminaBarRect.h*1.8f;

	int padding = 5;

	//two borders for thickness
	staminaOuterBorderRect.x = staminaBarRect.x - padding;
	staminaOuterBorderRect.y = staminaBarRect.y - padding;
	staminaOuterBorderRect.w = staminaBarMaxWidth + padding * 2;
	staminaOuterBorderRect.h = staminaBarRect.h + padding * 2;

	staminaInnerBorderRect.x = staminaBarRect.x - padding + 1;
	staminaInnerBorderRect.y = staminaBarRect.y - padding + 1;
	staminaInnerBorderRect.w = staminaBarMaxWidth + padding * 2;
	staminaInnerBorderRect.h = staminaBarRect.h + padding * 2;
	////////
	//hud color
	hudColor.a = 255;
	hudColor.b = 255;
	hudColor.r = 255;
	hudColor.g = 255;
	////////
	//hp
	hpNumberPixels[0] = 45;
	hpNumberPixels[1] = 72;
	lastHp = -1;

	// hp size & pos
	hpRect.h = hpNumberPixels[1];
	hpRect.x = hpNumberPixels[0];
	hpRect.y = resolution[1] - hpNumberPixels[1];
	////////
	//ammo
	reserveAmmoPixels[0] = 30;
	reserveAmmoPixels[1] = 48;

	clipRect.h = hpRect.h;
	clipRect.y = hpRect.y;
	
	reserveRect.h = reserveAmmoPixels[1];
	reserveRect.y = hpRect.y + (hpNumberPixels[1] - reserveAmmoPixels[1]);
}

SDL_Rect CHud::GetStaminaOuterBorderRect()
{
	return staminaOuterBorderRect;
}
int CHud::GetStaminaRedness()
{
	return vm->GetPlayer()->GetStaminaRedness();
}

SDL_Rect CHud::GetStaminaInnerBorderRect()
{
	return staminaInnerBorderRect;
}

SDL_Rect CHud::GetStaminaBarRect()
{
	staminaBarRect.w = staminaBarMaxWidth * (float)(vm->GetPlayer()->GetStamina() / 100.0f);
	return staminaBarRect;
}

CHud::~CHud()
{
	TTF_CloseFont(hpFont);
	TTF_CloseFont(dmgNumberFont);
	if (hpTexture != nullptr)
	{
		SDL_DestroyTexture(hpTexture);
		hpTexture = nullptr;
	}
	if (clipTexture != nullptr)
	{
		SDL_DestroyTexture(clipTexture);
		clipTexture = nullptr;
	}
	if (reserveTexture != nullptr)
	{
		SDL_DestroyTexture(reserveTexture);
		reserveTexture = nullptr;
	}
}

bool CHud::GetNextDamageNumber(SDL_Texture * & ptr, SDL_Rect & dest, int & alpha) //Gives pointer address to next damage number texture and fills the rect with size/ and _absolute_ dest, returns false if there are no more this "loop"
{
	std::vector<CDamageNumber*>* dmgVector = vm->GetDamageNumberVector();
	if (damageNumberIndex < damageNumberTextureMap.size())
	{
		ptr = damageNumberTextureMap.at(dmgVector->at(damageNumberIndex)); //is this stupid or clever?
		dest.h = damageNumberPixels[1];
		dest.w = dmgVector->at(damageNumberIndex)->ToString().length() * damageNumberPixels[0];
		dest.x = dmgVector->at(damageNumberIndex)->GetX();
		dest.y = dmgVector->at(damageNumberIndex)->GetY();
		alpha = dmgVector->at(damageNumberIndex)->GetColor().a;
		damageNumberIndex++;
		return true;
	}
	else
	{
		damageNumberIndex = 0;	//reset index if we've hit the roof
		return false;
	}
}

void CHud::UpdateAmmo()
{
	if (lastReserve != vm->GetPlayer()->GetCurrentReserveAmmo())
	{
		lastReserve = vm->GetPlayer()->GetCurrentReserveAmmo();

		SDL_DestroyTexture(reserveTexture); //destroy old texture

		//create new texture
		string reserveAmmoString = " / ";
		reserveAmmoString += std::to_string(lastReserve);

		SDL_Surface * tempSurface = TTF_RenderText_Blended(hpFont, reserveAmmoString.c_str(), hudColor);
		reserveTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		//calculate rect
		reserveRect.w = reserveAmmoString.length() * reserveAmmoPixels[0];
		reserveRect.x = resolution[0] - reserveRect.w - hpNumberPixels[0];

	}

	if (lastClip != vm->GetPlayer()->GetCurrentClip())
	{
		lastClip = vm->GetPlayer()->GetCurrentClip();

		SDL_DestroyTexture(clipTexture); //destroy old texture

		//create new texture
		string lastClipString = std::to_string(lastClip);
		SDL_Surface * tempSurface = TTF_RenderText_Blended(hpFont, lastClipString.c_str(), hudColor);
		clipTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		//calculate rect
		clipRect.w = lastClipString.length() * hpNumberPixels[0];
		clipRect.x = reserveRect.x - clipRect.w;
	}
}

void CHud::Update()
{
	UpdateDamageNumbers();
	UpdateHp();
	UpdateAmmo();
}

SDL_Rect CHud::GetClipRect()
{
	return clipRect;
}
SDL_Texture * CHud::GetClipTexture()
{
	return clipTexture;
}

SDL_Rect CHud::GetReserveAmmoRect()
{
	return reserveRect;
}

SDL_Texture * CHud::GetReserveAmmoTexture()
{
	return reserveTexture;
}


SDL_Texture * CHud::GetHpTexture()
{
	return hpTexture;
}
SDL_Rect CHud::GetHpRect()
{
	return hpRect;
}
void CHud::UpdateHp()
{
	if (vm->GetPlayer()->GetHP() != lastHp) //if hp has changed
	{
		lastHp = vm->GetPlayer()->GetHP();
		//set color to red if hp below 25
		if (lastHp < 25)
		{
			hudColor.b = 30;
			hudColor.g = 30;
		}
		else
		{
			hudColor.b = 255;
			hudColor.g = 255;
		}
		//set width of hpRectangle proportionate to number of numbers
		if (lastHp < 10)
		{
			hpRect.w = hpNumberPixels[0];
		}
		else if (lastHp < 100)
		{
			hpRect.w = hpNumberPixels[0] * 2;
		}
		else
		{
			hpRect.w = hpNumberPixels[0] * 3;
		}
		
		SDL_DestroyTexture(hpTexture); //destroy old texture
		//create new texture
		SDL_Surface * tempSurface = TTF_RenderText_Blended(hpFont, std::to_string(lastHp).c_str(), hudColor);
		hpTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
	}
}
void CHud::UpdateDamageNumbers()
{

	//check if there are new objects
	while (vm->GetDamageNumberVector()->size() > damageNumberTextureMap.size())
	{
		CDamageNumber * dmg = vm->GetDamageNumberVector()->at(damageNumberTextureMap.size()); //the size of textureMap is the index of the first unused damagenumber in the vector
		SDL_Surface * tempSurface = TTF_RenderText_Blended(dmgNumberFont, dmg->ToString().c_str(), dmg->GetColor()); //create temporary surface of the text (Text -> surface -> texture)
		damageNumberTextureMap[dmg] = SDL_CreateTextureFromSurface(renderer, tempSurface); //make surface into texture and store in map
		SDL_FreeSurface(tempSurface); // delete temporary surface
	}

	//delete expired objects
	for (int i = 0; i < vm->GetDamageNumberVector()->size(); i++)
	{
		if (vm->GetDamageNumberVector()->at(i)->Update()) //update dmg number; if damage number has expired
		{
			SDL_DestroyTexture(damageNumberTextureMap.at(vm->GetDamageNumberVector()->at(i))); //destroy texture associated with object
			damageNumberTextureMap.erase(vm->GetDamageNumberVector()->at(i)); //delete entry in map
			vm->DeleteObject(vm->GetDamageNumberVector()->at(i)); //delete number object
		}
	}

}