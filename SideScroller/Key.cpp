#include "Key.h"


CKey::CKey(int x, int y, int textureId, int _color) : CItem(x,y,textureId, 32)
{
	length = 32;
	height = 12;
	velocity.SetY(-0.1);
	frameSizeX =55;
	frameSizeY = 17;
	srand(SDL_GetTicks());
	float random = rand() % 10 + 1;
	random -= 5;
	velocity.SetX(random*0.02);
	color = _color;
}

void CKey::GetPickedUp(CPlayer * picker)
{
	picker->AddKey(color);
}
CKey::~CKey()
{
}
