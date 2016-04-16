#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H


#include "BaseObject.h"
class CPhysicsObject : public CBaseObject
{
public:
	CPhysicsObject(int x, int y, int textureId, int size);
	CPhysicsObject(int x, int y, int textureId, int size, bool subjectToGravity);
	CPhysicsObject(int x, int y, int _textureId, int width, int height, bool _subjectToGravity);

	virtual ~CPhysicsObject();
	int previousFramePos[2];
	void Update(int ms);
	float weight;

	void ApplyFriction(float slidynessOfSurface);

	/////////
	//not in use
	int hitbox;
	enum hitboxes
	{
		square,
		circle
	};
	//////////
	
	void AddVelocity(const CVector2f & relvel);


	CVector2f velocity;
	virtual void OnCollision(bool stopHor, bool stopVer) = 0;
	bool OnGround();
	void SetOnGround(bool);
	virtual void ApplyViscocityFriction(float viscocity);
	bool smallObject; // used for projectiles so they dont freeze animates
	bool underWater;
	
	bool IsSubjectToGravity();
	void SetSubjectToGravity(bool);

private:
	bool onGround;
	bool subjectToGravity = true;


};

#endif