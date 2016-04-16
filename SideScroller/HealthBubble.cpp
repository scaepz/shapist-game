#include "HealthBubble.h"
#include "SDL.h"

CHealthBubble::CHealthBubble(int x, int y, int textureId, int size, int _hp) : CItem(x,y,textureId, size)
{
	hp = _hp;
	velocity.SetY(-0.1);
	frameSizeX = 32;
	frameSizeY = 32;
	srand(SDL_GetTicks());
	float random = rand() % 10 + 1;
	random -= 5;
	velocity.SetX(random*0.02);
	
}


CHealthBubble::~CHealthBubble()
{
}

void CHealthBubble::GetPickedUp(CPlayer * picker)
{
	picker->AddHP(hp);
}

