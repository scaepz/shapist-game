#include "Line.h"


CLine::CLine()
{
	alphaLossPerMs = 1.0f;
	x1 = 0;
	x2 = 0;
	y1 = 1;
	y2 = 1;

	r = 1;
	g = 1;
	b = 1;
	alpha = 1;
}


CLine::~CLine()
{
}

bool CLine::Update()
{
	alpha -= alphaLossPerMs*g_time;
	if (alpha <= 0)
	{
		return true;
	}
	else return false;
}
void CLine::SetAlphaLossPerMs(float a)
{
	alphaLossPerMs = a;
	if (alphaLossPerMs < 0.25f)
	{
		alphaLossPerMs = 0.25f;
	}
}
void CLine::SetRGB(int _r, int _g, int _b)
{
	r = _r;
	if (r < 0) r = 0;
	if (r > 255) r = 255;


	g = _g;
	if (g < 0) g = 0;
	if (g > 255) g = 255;


	b = _b;
	if (b < 0) b = 0;
	if (b > 255) b = 255;
}
void CLine::SetAlpha(int _alpha)
{
	alpha = _alpha;
	if (alpha < 0) alpha = 0;
	if (alpha > 255) alpha = 255;
}