#ifndef PARTICLENGINE_H
#define PARTICLENGINE_H
#include <vector>
#include "Line.h"
using std::vector;
class CParticleEngine
{
public:
	void NewBulletTracer(int x1, int y1, int x2, int y2, float intensity);
	CParticleEngine();
	virtual ~CParticleEngine();
	vector<CLine*> lineVector;
	void Update();
	void DeleteAllParticles();
private:
	//tracers
	void UpdateLines();
	int standardLineSegmentLength;
	

};

#endif