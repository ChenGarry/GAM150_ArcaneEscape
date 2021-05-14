//------------------------------------------------------------------------------
//
// File Name:	BehaviorDummy.c
// Author(s):	Lorenzo DeMaine, Garry Chen 
// Project:		MyGame
// Course:		CS230S19B
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorDummy.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Random.h"
#include "MagneticPoint.h"
#include "Teleporter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the Dummy.
static const float DummySpeedMax = 0.0f;
// Maximum lifetime of a Dummy (in seconds).
static const float DummyFrozeTimeBase = 3.0f;
static const int DummyHealth = 10;
static const Vector2D moveVelocity = { 0.0f, 0.0f };
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef enum
{
	cDummyInvalid,
	cDummyIdle,
	cDummyFrozen,
	cDummyDead,
} DummyStates;

typedef struct DummyData
{
	Vector2D playerPos;
} DummyData;

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

// Dynamically allocate a new (Dummy) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorDummyCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	DummyData * DummyData = calloc(1, sizeof(DummyData));

	if (behavior && DummyData)
	{
		behavior->stateCurr = cDummyInvalid;
		behavior->stateNext = cDummyIdle;
		behavior->onInit = BehaviorDummyInit;
		behavior->onUpdate = BehaviorDummyUpdate;
		behavior->onExit = BehaviorDummyExit;
		behavior->cloneData = BehaviorDummycloneData;
		behavior->health = (float)DummyHealth;
		behavior->isAggro = false;
		behavior->isFrozen = false;
		behavior->inAir = true;
		behavior->isFacingRight = true;
		behavior->dataStructPtr = (void*)DummyData;
		behavior->aggroRange = 0.0f * 0.0f;
		behavior->timer = 0;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorDummyInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorDummyUpdate(BehaviorPtr behavior, float dt)
{
	TeleporterUpdateObject(behavior->parent);

	if (behavior->health <= 0)
		behavior->stateCurr = cDummyDead;
	else if (behavior->isFrozen)
	{
		behavior->isPatrol = false;
		behavior->stateNext = cDummyFrozen;
	}
		

	switch (behavior->stateCurr)
	{
		case cDummyIdle:
			BehaviorPoolSetYAcceleration(behavior->parent, -DummySpeedMax);
			BehaviorPoolOnFireTimer(behavior->parent, dt);
			OrbBehavior(behavior);	
			break;
		case cDummyFrozen:
			BehaviorPoolUnfreezeTimer(behavior->parent, dt);
			if (!behavior->isFrozen)
				behavior->stateNext = cDummyIdle;
			break;
		case cDummyDead:
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
void BehaviorDummyExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorDummyCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	UNREFERENCED_PARAMETER(gameObject);
	UNREFERENCED_PARAMETER(other);
	/*if (GameObjectIsNamed(other, "platform"))
		ObjectPlatformCollision(gameObject, other);
	else if (GameObjectIsNamed(other, "platform"))
		BehaviorDummyPlatformCollisionHandler(gameObject, other);*/
}

void BehaviorDummycloneData(BehaviorPtr behavior)
{
	DummyData * newDummyData = calloc(1, sizeof(DummyData));
	if (behavior && behavior->dataStructPtr && newDummyData)
	{
		*newDummyData = *(DummyData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newDummyData;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------