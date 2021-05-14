//------------------------------------------------------------------------------
//
// File Name:	BehaviorFloater.c
// Author(s):	Lorenzo DeMaine, Garry Chen 
// Project:		MyGame
// Course:		CS230S19B
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorFloater.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Random.h"
#include "MagneticPoint.h"
#include "Teleporter.h"
#include "Floater.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the Floater.
static const float FloaterSpeedMax = 1500.0f;
// Maximum lifetime of a Floater (in seconds).
static const float FloaterFrozeTimeBase = 3.0f;
static const int FloaterHealth = 3;
static const Vector2D moveVelocity = { 1100.0f, 0.0f };
static const float floatingTime = 4.96f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef enum
{
	cFloaterInvalid,
	cFloaterIdle,
	cFloaterRecover,
	cFloaterPattrol,
	cFloaterFrozen,
	cFloaterAggro,
	cFloaterDead,
} FloaterStates;

typedef struct FloaterData
{
	Vector2D playerPos;
} FloaterData;

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

// Dynamically allocate a new (Floater) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorFloaterCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	FloaterData * FloaterData = calloc(1, sizeof(FloaterData));

	if (behavior && FloaterData)
	{
		behavior->stateCurr = cFloaterInvalid;
		behavior->stateNext = cFloaterIdle;
		behavior->onInit = BehaviorFloaterInit;
		behavior->onUpdate = BehaviorFloaterUpdate;
		behavior->onExit = BehaviorFloaterExit;
		behavior->cloneData = BehaviorFloatercloneData;
		behavior->health = (float)FloaterHealth;
		behavior->isAggro = false;
		behavior->isFrozen = false;
		behavior->inAir = true;
		behavior->isFacingRight = true;
		behavior->dataStructPtr = (void*)FloaterData;
		behavior->aggroRange = 350.0f * 350.0f;
		behavior->timer = 0;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorFloaterInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorFloaterUpdate(BehaviorPtr behavior, float dt)
{
	PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
	Vector2D floatvelocity = *PhysicsGetVelocity(physics);

	TeleporterUpdateObject(behavior->parent);

	if (floatvelocity.x != 0)
		FloaterSetState(behavior->parent, FloaterMove);

	if (behavior->health <= 0)
	{
		FloaterSetState(behavior->parent, FloaterHurt);
		behavior->stateCurr = cFloaterDead;
	}
	else if (behavior->isFrozen)
	{
		behavior->isPatrol = false;
		behavior->stateNext = cFloaterFrozen;
		FloaterSetState(behavior->parent, FloaterHurt);
	}
		

	switch (behavior->stateCurr)
	{
		case cFloaterIdle:
			behavior->stateNext = cFloaterPattrol;
			Vector2D velocity;
			velocity.x = 0;
			velocity.y = FloaterSpeedMax/2;
			BehaviorPoolMove(behavior->parent, &velocity);
			BehaviorPoolSetYAcceleration(behavior->parent, -FloaterSpeedMax);
			BehaviorPoolOnFireTimer(behavior->parent, dt);
			OrbBehavior(behavior);
			behavior->timer = floatingTime;
			behavior->isPatrol = true;	
			break;
		case cFloaterPattrol:
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
			{
				behavior->stateNext = cFloaterRecover;
				behavior->timer = 0.5f;
				behavior->isPatrol = false;
			}
			break;
		case cFloaterAggro:
			break;
		case cFloaterRecover:
			BehaviorPoolStop(behavior->parent);
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
				behavior->stateNext = cFloaterIdle;
			break;
		case cFloaterFrozen:
			BehaviorPoolUnfreezeTimer(behavior->parent, dt);
			if (!behavior->isFrozen)
				behavior->stateNext = cFloaterIdle;
			break;
		case cFloaterDead:
			GameObjectDestroy(behavior->parent);
			break;
	}

	BehaviorPoolChangeSide(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorFloaterExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorFloaterCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	UNREFERENCED_PARAMETER(gameObject);
	UNREFERENCED_PARAMETER(other);
	/*if (GameObjectIsNamed(other, "platform"))
		ObjectPlatformCollision(gameObject, other);
	else if (GameObjectIsNamed(other, "platform"))
		BehaviorFloaterPlatformCollisionHandler(gameObject, other);*/
}

void BehaviorFloatercloneData(BehaviorPtr behavior)
{
	FloaterData * newFloaterData = calloc(1, sizeof(FloaterData));
	if (behavior && behavior->dataStructPtr && newFloaterData)
	{
		*newFloaterData = *(FloaterData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newFloaterData;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------