#ifndef LINE_H
#define LINE_H
#include "Global.h"
class CLine
{
public:
	CLine();
	virtual ~CLine();
	float alphaLossPerMs;
	int x1, y1, x2, y2;
	bool Update();
	void SetRGB(int r, int g, int b);
	void SetAlpha(int alpha);
	void SetAlphaLossPerMs(float);
	int r, g, b;
	float alpha;

};

#endif