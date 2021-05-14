//------------------------------------------------------------------------------
//
// File Name:	BehaviorFlower.c
// Author(s):	Garry Chen, QIAOCHU LIU
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorFlower.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "GameObject.h"
#include "MagneticPoint.h"
#include "Vector2D.h"
#include "HealthSystem.h"
#include "Flower.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the Flower.
static const float FlowerSpeedMax = 150.0f;
static const float MonsterFireBallSpeedMax = 500.0f;
// Maximum lifetime of a Flower (in seconds).
static const float FlowerFrozeTimeBase = 3.0f;
static const int FlowerHealth = 20;
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
	cFlowerInvalid,
	cFlowerIdle,
	cFlowerRecover,
	cFlowerPattrol,
	cFlowerShooting,
	cFlowerFrozen,
	cFlowerAggro,
	cFlowerDead,
} FlowerStates;

typedef struct FlowerData
{
	Vector2D playerPos;
} FlowerData;

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

// Dynamically allocate a new (Flower) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorFlowerCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	FlowerData * FlowerData = calloc(1, sizeof(FlowerData));

	if (behavior && FlowerData)
	{
		behavior->stateCurr = cFlowerInvalid;
		behavior->stateNext = cFlowerIdle;
		behavior->onInit = BehaviorFlowerInit;
		behavior->onUpdate = BehaviorFlowerUpdate;
		behavior->onExit = BehaviorFlowerExit;
		behavior->cloneData = BehaviorFlowercloneData;
		behavior->health = (float)FlowerHealth;
		behavior->isAggro = false;
		behavior->isFrozen = false;
		behavior->inAir = true;
		behavior->isFacingRight = true;
		behavior->dataStructPtr = (void*)FlowerData;
		behavior->aggroRange = 350.0f * 350.0f;
		behavior->timer = 0;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorFlowerInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorFlowerUpdate(BehaviorPtr behavior, float dt)
{

	
	if (behavior->health <= 0)
	{
		FlowerSetState(behavior->parent, FlowerDie);
		behavior->stateCurr = cFlowerDead;
	}
	else if (behavior->isFrozen)
	{
		behavior->isPatrol = false;
		behavior->stateNext = cFlowerFrozen;
		FlowerSetState(behavior->parent, FlowerHurt);
	}

	if(!behavior->isFrozen)
		FlowerSetState(behavior->parent, FlowerIdle);

	switch (behavior->stateCurr)
	{
		case cFlowerIdle:
			if(behavior->isAggro)
				behavior->stateNext = cFlowerAggro;

			BehaviorPoolOnFireTimer(behavior->parent, dt);
			
			/*else
			{
				behavior->stateNext = cFlowerPattrol;
				if(behavior->isFacingRight)
					BehaviorPoolMove(behavior->parent, &moveRightVelocity);
				else
					BehaviorPoolMove(behavior->parent, &moveLeftVelocity);
				behavior->isPatrol = true;
			}		*/	
			break;
		case cFlowerPattrol:
			if (behavior->isAggro)
			{
				behavior->stateNext = cFlowerAggro;
				behavior->isPatrol = false;
			}
			break;
		case cFlowerAggro:
			if (behavior->targetPos.x > TransformGetTranslation(GameObjectGetTransform(behavior->parent))->x)
			{
				behavior->isFacingRight = true;
			}
			else
			{
				behavior->isFacingRight = false;
			}
			behavior->stateNext = cFlowerShooting;
			break;
		case cFlowerShooting:
			BehaviorPoolShoot(behavior->parent, "MonsterFireBall", &behavior->targetPos, MonsterFireBallSpeedMax);
			behavior->timer = 1.5f;
			behavior->stateNext = cFlowerRecover;
			break;
		case cFlowerRecover:
			BehaviorPoolUpdateTimer(behavior, dt);
			if (behavior->timer <= 0)
				behavior->stateNext = cFlowerIdle;
			break;
		case cFlowerFrozen:
			BehaviorPoolUnfreezeTimer(behavior->parent, dt);
			if (!behavior->isFrozen)
				behavior->stateNext = cFlowerIdle;
			break;
		case cFlowerDead:
			GameObjectDestroy(behavior->parent);
			break;
	}

	BehaviorPoolChangeSide(behavior->parent);
	PhysicsSetAcceleration(GameObjectGetPhysics(behavior->parent), &gravityNone);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorFlowerExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorFlowerCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	GameObjectDestroy(gameObject);
	UNREFERENCED_PARAMETER(other);
	//if (strcmp(GameObjectGetName(other), "Player") == 0)
	//{
	//	//MonsterSpellColldeHandler();
	//}
}

void BehaviorFlowercloneData(BehaviorPtr behavior)
{
	FlowerData * newFlowerData = calloc(1, sizeof(FlowerData));
	if (behavior && behavior->dataStructPtr && newFlowerData)
	{
		*newFlowerData = *(FlowerData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newFlowerData;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------