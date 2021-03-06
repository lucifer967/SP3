#pragma once
#include "Vector3.h"

class CCollider
{
public:
	CCollider();
	virtual ~CCollider();

	// Set the maxAABB and minAABB
	virtual void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	// Set the maxAABB
	virtual void SetMaxAABB(Vector3 maxAABB);
	// Get the maxAABB
	virtual Vector3 GetMaxAABB(void);
	// Set the minAABB
	virtual void SetMinAABB(Vector3 minAABB);
	// Get the minAABB
	virtual Vector3 GetMinAABB(void);
	//Collision
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	//Player AABB
	virtual void SetPAABB(Vector3 size, Vector3 pos);

protected:
	// Vector3 variables to store coordinates of the AABB bounding box of an object
	Vector3 maxAABB;
	Vector3 minAABB;
};
