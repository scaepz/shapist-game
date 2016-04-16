#include "TrapHandler.h"


CTrapHandler::CTrapHandler(CVectorManager * _vm)
{
	vm = _vm;
	animateVector = vm->GetAnimateVector();
	trapVector = vm->GetTrapVector();
}


CTrapHandler::~CTrapHandler()
{
}

void CTrapHandler::HandleTraps()
{
	CCollisionDetector cd;
	for (int i = 0; i < trapVector->size(); i++)
	{
		for (int o = 0; o < animateVector->size(); o++)
		{
			if (cd.Collides(trapVector->at(i), animateVector->at(o)))
			{
				trapVector->at(i)->AffectVictim(animateVector->at(o));
			}
		}
	}
}