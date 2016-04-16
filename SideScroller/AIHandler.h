#ifndef AIHANDLER_H
#define AIHANDLER_H
#include "VectorManager.h"
#include "AI.h"
#include "RetreatingAI.h"
#include "AggressiveAI.h"
#include "RangedAI.h"
#include "CautiousRangedAI.h"
class CAIHandler
{
public:
	CAIHandler(CVectorManager * _vm);
	virtual ~CAIHandler();
	void HandleAI();
private:
	CVectorManager * vm;
	std::vector<CAI*> AIVector;
};

#endif