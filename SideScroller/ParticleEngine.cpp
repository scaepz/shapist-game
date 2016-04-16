#include "ParticleEngine.h"


CParticleEngine::CParticleEngine()
{
	standardLineSegmentLength = 200;
}


CParticleEngine::~CParticleEngine()
{
}

void CParticleEngine::NewBulletTracer(int x1, int y1, int x2, int y2, float intensity)
{
	int orderedTracerLength = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1) * (y2 - y1));
	int createdTracerLength = 0;
	bool firstLine = true;
	float gradient = (float)(y2 - y1) / (float)(x2 - x1);
	float lineSegmentLength = standardLineSegmentLength * intensity;
	int loopNumber = 0;

	//create a bunch of lines with decreasing color and alpha intensity 
	while (createdTracerLength < orderedTracerLength)
	{
		float angle = atan2(y2 - y1, x2 - x1);
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		CLine * line = new CLine();
		
		if (firstLine)
		{
			firstLine = false;
			line->x1 = x1;
			line->y1 = y1;
		}
		else
		{
			//take first point coordinates from last line's second point coordinates
			line->x1 = lineVector.at(lineVector.size() - 1)->x2 + cosAngle/abs(cosAngle);
			line->y1 = lineVector.at(lineVector.size() - 1)->y2 + sinAngle/abs(sinAngle);

		}
		 
		if (createdTracerLength + lineSegmentLength > orderedTracerLength) //last line is shorter
		{
			lineSegmentLength = orderedTracerLength - createdTracerLength;
		}
		/*x = old_x + length * cos(alpha);
		y = old_y + length * sin(alpha);*/

		//calculate x2 and y2
		
		line->x2 = line->x1 + lineSegmentLength * cosAngle;
		line->y2 = line->y1 + lineSegmentLength * sinAngle;
		
		createdTracerLength += lineSegmentLength;

		//set color values according to intensity
		line->SetRGB(255, 255, 255 - (intensity*150.0f) + loopNumber * (8.0f / intensity));
		line->SetAlpha(160 + (intensity*50.0f) - (loopNumber * (5.0f / intensity)));
		line->SetAlphaLossPerMs(1.3f - intensity * 2);
		loopNumber++;
		lineVector.push_back(line);
	}
}

void CParticleEngine::Update()
{
	UpdateLines();
}


void CParticleEngine::UpdateLines()
{
	for (int i = 0; i < lineVector.size(); i++)
	{
		if (lineVector.at(i)->Update())		//Updates alpha and returns true if alpha is 0
		{
			delete lineVector.at(i);		//object is then deleted
			lineVector.erase(lineVector.begin() + i);
			i--;
		}
	}
}

void CParticleEngine::DeleteAllParticles()
{
	while (lineVector.size() > 0)
	{
		delete lineVector[0];
		lineVector.erase(lineVector.begin());
	}
}