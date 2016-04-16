#include "PhysicsObject.h"
#include <iostream>
#include "Global.h"

CPhysicsObject::CPhysicsObject(int x, int y, int _textureId, int _size) : CBaseObject(x,y,_textureId, _size)
{
	velocity[0] = 0;
	velocity[1] = 0;
}
CPhysicsObject::CPhysicsObject(int x, int y, int _textureId, int _size, bool _subjectToGravity) : CBaseObject(x, y, _textureId, _size)
{
	velocity[0] = 0;
	velocity[1] = 0;
	subjectToGravity = _subjectToGravity;
}

CPhysicsObject::CPhysicsObject(int x, int y, int _textureId, int width, int height, bool _subjectToGravity) : CBaseObject(x, y, _textureId, width, height)
{
	velocity[0] = 0;
	velocity[1] = 0;
	subjectToGravity = _subjectToGravity;
}

void CPhysicsObject::ApplyFriction(float slidynessOfSurface)
{
	if (velocity[0] < 0.003 && velocity[0] > 0 || velocity[0] > -0.003 && velocity[0] < 0)
	{
		velocity[0] = 0;
	}
	else
	{
		for (int i = 0; i < g_time; i++)
		{
			velocity[0] *= slidynessOfSurface;
		}
	}
}
CPhysicsObject::~CPhysicsObject()
{
}

bool CPhysicsObject::IsSubjectToGravity()
{
	return subjectToGravity;
}
void CPhysicsObject::SetSubjectToGravity(bool b)
{
	subjectToGravity = b;
}


void CPhysicsObject::AddVelocity(const CVector2f & relvel)
{
	velocity[0] += relvel[0];
	velocity[1] += relvel[1];
}

void CPhysicsObject::ApplyViscocityFriction(float viscocity)
{
	velocity[0] *= viscocity;
	velocity[1] *= viscocity;
}


bool CPhysicsObject::OnGround()
{
	return onGround;
}
void CPhysicsObject::SetOnGround(bool newState)
{
	onGround = newState;
}

void CPhysicsObject::Update(int ms)
{
	previousFramePos[0] = pos[0];
	previousFramePos[1] = pos[1];
	pos[0] += velocity[0] * ms;
	pos[1] += velocity[1] * ms;
}