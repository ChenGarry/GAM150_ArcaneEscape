//------------------------------------------------------------------------------
//
// File Name:	BehaviorKnight.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorKnight.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "GameObject.h"
#include "MagneticPoint.h"
#include "Vector2D.h"
#include "MagneticPoint.h"
#include "Knight.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the Knight.
static const float KnightSpeedMax = 150.0f;
// Maximum lifetime of a Knight (in seconds).
static const float KnightFrozeTimeBase = 1.0f;
static const int KnightHealth = 20;
static const Vector2D gravityNormal = { 0.0f, -25000.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D moveRightVelocity = { 350.0f, 0.0f };
static const Vector2D moveLeftVelocity = { -350.0f, 0.0f };
static const Vector2D chargeRightVelocity = { 1000.0f, 0.0f };
static const Vector2D chargeLeftVelocity = { -1000.0f, 0.0f };
static Vector2D jumpVelocity = { -600.0f, 3000.0f };

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum
{
	cKnightInvalid,
	cKnightIdle,
	cKnightRecover,
	cKnightPattrol,
	cKnightCharging,
	cKnightFrozen,
	cKnightAggro,
	cKnightMagnitized,
	cKnightDead,
} KnightStates;

typedef struct KnightData
{
	Vector2D playerPos;
} KnightData;

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

// Dynamically allocate a new (Knight) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorKnightCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	KnightData * KnightData = calloc(1, sizeof(KnightData));

	if (behavior && KnightData)
	{
		behavior->stateCurr = cKnightInvalid;
		behavior->stateNext = cKnightIdle;
		behavior->onInit = BehaviorKnightInit;
		behavior->onUpdate = BehaviorKnightUpdate;
		behavior->onExit = BehaviorKnightExit;
		behavior->cloneData = BehaviorKnightcloneData;
		behavior->health = (float)KnightHealth;
		behavior->isAggro = false;
		behavior->isFrozen = false;
		behavior->inAir = true;
		behavior->isFacingRight = true;
		behavior->isMetal = true;
		behavior->dataStructPtr = (void*)KnightData;
		behavior->aggroRange = 350.0f * 350.0f;
		behavior->timer = 0;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorKnightInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorKnightUpdate(BehaviorPtr behavior, float dt)
{
	PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
	Vector2D velocity = *PhysicsGetVelocity(physics);

	if(velocity.x == 0)
		KnightSetState(behavior->parent, KnightIdle);
	else
		KnightSetState(behavior->parent, KnightMove);

	if (behavior->health <= 0)
	{
		KnightSetState(behavior->parent, KnightDie);
		behavior->stateCurr = cKnightDead;
	}
	else if (behavior->isFrozen)
	{
		behavior->isPatrol = false;
		behavior->stateNext = cKnightFrozen;
	}
	else if(behavior->isMagnitized)
	{
		BehaviorPoolUpdateMagTimer(behavior, dt);
		if (behavior->MagTimer == 0)
		{
			behavior->isMagnitized = false;
			BehaviorPoolSetYAcceleration(behavior->parent, -25000.f);
		}
		else
		{
			OrbsvsEnemy(behavior->parent);
		}
		KnightSetState(behavior->parent, KnightMove);
	}
		

	switch (behavior->stateCurr)
	{
		case cKnightIdle:
			if(behavior->isAggro)
				behavior->stateNext = cKnightAggro;
			else
			{
				behavior->stateNext = cKnightPattrol;
				if(behavior->isFacingRight)
					BehaviorPoolMove(behavior->parent, &moveRightVelocity);
				else
					BehaviorPoolMove(behavior->parent, &moveLeftVelocity);
				behavior->isPatrol = true;
			}		

			BehaviorPoolOnFireTimer(behavior->parent, dt);
			break;
		case cKnightPattrol:
			if (behavior->isAggro)
			{
				behavior->stateNext = cKnightAggro;
				behavior->isPatrol = false;
			}
			break;
		case cKnightAggro:
			if (behavior->targetPos.x > TransformGetTranslation(GameObjectGetTransform(behavior->parent))->x)
			{
				behavior->isFacingRight = true;
				BehaviorPoolMove(behavior->parent, &chargeRightVelocity);
			}
			else
			{
				behavior->isFacingRight = false;
				BehaviorPoolMove(behavior->parent, &chargeLeftVelocity);
			}
			behavior->timer = 3.0f;
			behavior->stateNext = cKnightCharging;
			break;
		case cKnightCharging:
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
			{
				behavior->stateNext = cKnightRecover;
				behavior->timer = 3.0f;
			}				
			break;
		case cKnightRecover:
			BehaviorPoolSetXVolicity(behavior->parent, 0);
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
				behavior->stateNext = cKnightIdle;
			break;
		case cKnightFrozen:
			BehaviorPoolUnfreezeTimer(behavior->parent, dt);
			if (!behavior->isFrozen)
				behavior->stateNext = cKnightIdle;
			break;
		case cKnightMagnitized:
			if (!behavior->isMagnitized)
			{
				behavior->stateNext = cKnightIdle;
				BehaviorPoolSetYAcceleration(behavior->parent, -25000.f);
			}
		case cKnightDead:
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
void BehaviorKnightExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorKnightCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	UNREFERENCED_PARAMETER(gameObject);
	UNREFERENCED_PARAMETER(other);
	/*if (GameObjectIsNamed(other, "platform"))
		ObjectPlatformCollision(gameObject, other);
	else if (GameObjectIsNamed(other, "platform"))
		BehaviorKnightPlatformCollisionHandler(gameObject, other);*/
}

void BehaviorKnightcloneData(BehaviorPtr behavior)
{
	KnightData * newKnightData = calloc(1, sizeof(KnightData));
	if (behavior && behavior->dataStructPtr && newKnightData)
	{
		*newKnightData = *(KnightData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newKnightData;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------