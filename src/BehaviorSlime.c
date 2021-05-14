//------------------------------------------------------------------------------
//
// File Name:	BehaviorSlime.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSlime.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "MagneticPoint.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Slime.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the Slime.
static const float SlimeSpeedMax = 150.0f;
// Maximum lifetime of a Slime (in seconds).
static const float SlimeFrozeTimeBase = 3.0f;
static const int slimeHealth = 5;
static const Vector2D gravityNormal = { 0.0f, -25000.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D moveRightVelocity = { 350.0f, 0.0f };
static const Vector2D moveLeftVelocity = { -350.0f, 0.0f };
static Vector2D jumpVelocity = { -600.0f, 3000.0f };



//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef enum
{
	cSlimeInvalid,
	cSlimeIdle,
	cSlimeRecover,
	cSlimePattrol,
	cSlimeJumping,
	cSlimeFrozen,
	cSlimeAggro,
	cSlimeDead,
} SlimeStates;

typedef struct SlimeData
{
	Vector2D playerPos;
} SlimeData;

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

// Dynamically allocate a new (Slime) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorSlimeCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	SlimeData * slimeData = calloc(1, sizeof(SlimeData));

	if (behavior && slimeData)
	{
		behavior->stateCurr = cSlimeInvalid;
		behavior->stateNext = cSlimeIdle;
		behavior->onInit = BehaviorSlimeInit;
		behavior->onUpdate = BehaviorSlimeUpdate;
		behavior->onExit = BehaviorSlimeExit;
		behavior->cloneData = BehaviorSlimecloneData;
		behavior->health = (float)slimeHealth;
		behavior->isAggro = false;
		behavior->isFrozen = false;
		behavior->inAir = true;
		behavior->isFacingRight = true;
		behavior->dataStructPtr = (void*)slimeData;
		behavior->aggroRange = 350.0f * 350.0f;
		behavior->timer = 0;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSlimeInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSlimeUpdate(BehaviorPtr behavior, float dt)
{
	PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
	Vector2D velocity = *PhysicsGetVelocity(physics);

	if(velocity.x != 0 && !behavior->inAir && !behavior->isFrozen)
		SlimeSetState(behavior->parent, SlimeMove);
	else if(velocity.y > 0)
		SlimeSetState(behavior->parent, SlimeJump);
	else if(velocity.y < 0)
		SlimeSetState(behavior->parent, SlimeFall);
	else
		SlimeSetState(behavior->parent, SlimeIdle);

	if (behavior->health <= 0)
	{
		SlimeSetState(behavior->parent, SlimeHurt);
		behavior->stateCurr = cSlimeDead;
	}
	else if (behavior->isFrozen)
	{
		SlimeSetState(behavior->parent, SlimeHurt);
		behavior->isPatrol = false;
		behavior->stateNext = cSlimeFrozen;
	}
		

	switch (behavior->stateCurr)
	{

		case cSlimeIdle:
			if (behavior->isAggro)
				behavior->stateNext = cSlimeAggro;
			else
			{
				behavior->stateNext = cSlimePattrol;
				if(behavior->isFacingRight)
					BehaviorPoolMove(behavior->parent, &moveRightVelocity);
				else
					BehaviorPoolMove(behavior->parent, &moveLeftVelocity);
				behavior->isPatrol = true;
			}		

			BehaviorPoolOnFireTimer(behavior->parent, dt);
			break;
		case cSlimePattrol:
			if (behavior->isAggro)
			{
				behavior->stateNext = cSlimeAggro;
				behavior->isPatrol = false;
			}
			break;
		case cSlimeAggro:
			if (behavior->targetPos.x > TransformGetTranslation(GameObjectGetTransform(behavior->parent))->x)
			{
				behavior->isFacingRight = true;
				jumpVelocity.x = 800.0f;
			}
			else
			{
				behavior->isFacingRight = false;
				jumpVelocity.x = -800.0f;
			}
			BehaviorPoolJump(behavior->parent, &jumpVelocity, &gravityNormal);
			behavior->stateNext = cSlimeJumping;
			break;
		case cSlimeJumping:
			if (!behavior->inAir)
			{
				behavior->stateNext = cSlimeRecover;
				behavior->timer = 3.0f;
			}				
			break;
		case cSlimeRecover:
			BehaviorPoolSetXVolicity(behavior->parent, 0);
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
				behavior->stateNext = cSlimeIdle;
			break;
		case cSlimeFrozen:
			BehaviorPoolUnfreezeTimer(behavior->parent, dt);
			if (!behavior->isFrozen)
				behavior->stateNext = cSlimeIdle;
			break;
		case cSlimeDead:
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
void BehaviorSlimeExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorSlimeCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	UNREFERENCED_PARAMETER(gameObject);
	UNREFERENCED_PARAMETER(other);
	/*if (GameObjectIsNamed(other, "platform"))
		ObjectPlatformCollision(gameObject, other);
	else if (GameObjectIsNamed(other, "platform"))
		BehaviorSlimePlatformCollisionHandler(gameObject, other);*/
}

void BehaviorSlimecloneData(BehaviorPtr behavior)
{
	SlimeData * newSlimeData = calloc(1, sizeof(SlimeData));
	if (behavior && behavior->dataStructPtr && newSlimeData)
	{
		*newSlimeData = *(SlimeData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newSlimeData;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------