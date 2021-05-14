//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEMath.h"
#include "AEEngine.h"
#include "Trace.h"
#include "Collider.h"
#include "Collision.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct Collider
{
	// Pointer to a function that handles collisions between two objects.
	CollisionEventHandler	handler;

	CollidingWherePtr PlatformCollision;
	// Pointer to the collider's parent game object.
	GameObjectPtr parent;

} Collider;



//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
ColliderPtr ColliderCreate(GameObjectPtr parent)
{
	ColliderPtr collider = calloc(1, sizeof(Collider));
	if (collider)
	{
		CollidingWherePtr collisionWhere = calloc(1, sizeof(CollidingWhere));
		collider->parent = parent;
		if (collisionWhere)
		{
			collider->PlatformCollision = collisionWhere;
			collider->PlatformCollision->collidingBot = false;
			collider->PlatformCollision->collidingTop = false;
			collider->PlatformCollision->collidingRight = false;
			collider->PlatformCollision->collidingLeft = false;
		}
	}
	return collider;
}

// Dynamically allocate a clone of an existing collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
//   parent = Pointer to the cloned component's parent.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
ColliderPtr ColliderClone(const ColliderPtr other, GameObjectPtr parent)
{
	if (other)
	{
		ColliderPtr collider = calloc(1, sizeof(Collider));
		if (collider)
		{
			*collider = *other;
			collider->parent = parent;
		}
		return collider;
	}
	return NULL;
}

// Free the memory associated with a collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the collider component.
void ColliderFree(ColliderPtr * collider)
{
	free(*collider);
	*collider = NULL;
}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first collider component.
//	 collider2 = Pointer to the second collider component.
void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	if (AABBisCollidingAll(collider->parent, other->parent))
	{
		if(collider->handler)
			collider->handler(collider->parent, other->parent);
		if(other->handler)
			other->handler(other->parent, collider->parent);
	}
}

CollisionEventHandler ColliderGetCollisionHandler(ColliderPtr collider)
{
	if (collider)
	{
		return collider->handler;
	}
	return NULL;
}
// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(ColliderPtr collider, CollisionEventHandler handler)
{
	if (collider)
	{
		collider->handler = handler;
	}
}

GameObjectPtr ColliderGetParent(ColliderPtr collider)
{
	if (collider)
		return collider->parent;
	return NULL;
}

CollidingWherePtr ColliderGetPlatformCollision(ColliderPtr collider)
{
	if (collider)
		return collider->PlatformCollision;
	return NULL;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
