#include "DamageNumber.h"

CDamageNumber::CDamageNumber(int damage, int startingPosX, int startingPosY)
{
	font = 0;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255; 
	value = "";
	if (damage >= 0 && damage < 10) //white
	{
		color.r = 255;
		color.b = 255;
		color.g = 255;
	}

	else if (damage >= 10 && damage < 30) // yellow
	{
		color.b = 0;
	}
	else if (damage >= 30 && damage < 70) //orange
	{
		color.b = 25;
		color.g = 110;
	}
	else if (damage >= 70) //red
	{
		color.b = 0;
		color.g = 0;
	}
	else //green
	{
		color.b = 0;
		color.g = 255;
		color.r = 0;
		damage *= -1;
		value = "+";
	}

	
	
	timeAlive = 2;
	value += std::to_string(damage);
	position[0] = startingPosX;
	position[1] = startingPosY;

	velocity[0] = rand() % 100; 
	velocity[0] -= 50;
	velocity[0] /= 500;
	velocity[1] = -0.4f + (float)(((rand() % 10) -5 ) / 100.0f) ;
}

CDamageNumber::~CDamageNumber()
{
}
int CDamageNumber::GetFontIndex()
{
	return font;
}
bool CDamageNumber::Update()
{
	timeAlive += g_time; //update time since spawn
	if (timeAlive > 800)
	{
		return true;
	}
	if (timeAlive / 550 != 0) 
	{
		color.a = 200 / (timeAlive / 200);
	}
	velocity[1] += (float)g_time * 0.0011f; //add gravity

	position[0] += velocity[0] * g_time; //update position
	position[1] += velocity[1] * g_time;
	return false;
	
}

SDL_Color CDamageNumber::GetColor()
{
	return color;
}

string CDamageNumber::ToString()
{
	return value;
}
int CDamageNumber::GetX()
{
	return position[0];
}
int CDamageNumber::GetY()
{
	return position[1];
}