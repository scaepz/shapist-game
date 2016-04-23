#include "Weapon.h"
#include "SDL.h"
#include "SDL_image.h"
#include <fstream>
#include "FileReader.h"
CWeapon::CWeapon()
{
	flipPoint.x = 0;
	flipPoint.y = 0;
	animPointOfNoReturnInMs = -1;
	positionAdjustment[0] = 0;
	positionAdjustment[1] = 0;
	idlePositionAdjustment[0] = 0;
	idlePositionAdjustment[1] = 8;
	idleRotation = 15;
}


CWeapon::~CWeapon()
{

}

void CWeapon::CeaseFire()
{
	return;
}

void CWeapon::Update()
{

}
int CWeapon::GetFrame(int ms, int anim)
{
	return 0;
}
int CWeapon::GetFrameSizeX()
{
	return frameSizeX;
}
int CWeapon::GetFrameSizeY()
{
	return frameSizeY;
}

void CWeapon::InitHandPlacement()
{
	//load the weapons texture
	std::fstream reader("res/textures/standardTextures.txt");
	string textureData;
	CFileReader rowreader;
	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		textureData += temp;
	}
	reader.close();
	SDL_Surface *surface = NULL;


	for (int i = 0; rowreader.GetRow(textureData, i) != "eof"; i += 2)
	{
		if (atoi(rowreader.GetRow(textureData, i).c_str()) == textureId)
		{
			surface = IMG_Load(rowreader.GetRow(textureData, i+1).c_str());
			break;
		}
	}
	if (surface == NULL) return;
	//find purple pixels
	SDL_PixelFormat *fmt;
	fmt = surface->format;
	SDL_LockSurface(surface);
	Uint32 *pixels = ((Uint32*)surface->pixels);
	SDL_UnlockSurface(surface);

	for (int x = 0; x < surface->w; x++)
	{
		for (int y = 0; y < surface->h; y++)
		{
			Uint32 temp, pixel;
			Uint8 red, green, blue, alpha;
			pixel = pixels[(y * surface->w) + x];
			/* Get Red component */
			temp = pixel & fmt->Rmask;  /* Isolate red component */
			temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
			red = (Uint8)temp;

			/* Get Green component */
			temp = pixel & fmt->Gmask;  /* Isolate green component */
			temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
			green = (Uint8)temp;

			/* Get Blue component */
			temp = pixel & fmt->Bmask;  /* Isolate blue component */
			temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
			temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
			blue = (Uint8)temp;

			if (red == 255 && blue == 255 && green == 0)
			{
				//if purple pixel found, add its coordinates to handplacements. CArtist will render an animate's hands at these positions.
				handPlacements.push_back(std::pair<int,int>(x,y));
			}
		}
	}

}