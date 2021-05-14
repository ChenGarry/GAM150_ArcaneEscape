//------------------------------------------------------------------------------
//
// File Name:	BehaviorMonsterFireBall.c
// Author(s):	Garry Chen
// Project:		MyGame
// Course:		CS230S19B
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorMonsterFireBall.h"
#include "BehaviorPool.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "HealthSystem.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

// Maximum speed of the MonsterFireBall.
static const float MonsterFireBallSpeedMax = 400.0f;
static const float MonsterFireBallTimeMax = 30.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef enum
{
	cMonsterFireBallInvalid,
	cMonsterFireBallIdle,
} MonsterFireBallStates;

typedef struct MonsterFireBallData
{
	Vector2D playerPos;
} MonsterFireBallData;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorMonsterFireBallUpdateLifeTimer(BehaviorPtr behavior, float dt);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (MonsterFireBall) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorMonsterFireBallCreate()
{
	BehaviorPtr behavior = calloc(1, sizeof(Behavior));
	MonsterFireBallData * MonsterFireBallData = calloc(1, sizeof(MonsterFireBallData));

	if (behavior && MonsterFireBallData)
	{
		behavior->stateCurr = cMonsterFireBallInvalid;
		behavior->stateNext = cMonsterFireBallIdle;
		behavior->onInit = BehaviorMonsterFireBallInit;
		behavior->onUpdate = BehaviorMonsterFireBallUpdate;
		behavior->onExit = BehaviorMonsterFireBallExit;
		behavior->cloneData = BehaviorMonsterFireBallcloneData;
		behavior->timer = MonsterFireBallTimeMax;
	}
	return behavior;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorMonsterFireBallInit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
	if (behavior->stateCurr == cMonsterFireBallIdle)
	{
		ColliderSetCollisionHandler(GameObjectGetCollider(behavior->parent), BehaviorMonsterFireBallCollisionHandler);		
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorMonsterFireBallUpdate(BehaviorPtr behavior, float dt)
{
	switch (behavior->stateCurr)
	{
		case cMonsterFireBallIdle:
			BehaviorMonsterFireBallUpdateLifeTimer(behavior, dt);
			break;
	}

}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorMonsterFireBallExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

void BehaviorMonsterFireBallCollisionHandler(GameObjectPtr gameObject, GameObjectPtr other)
{
	if (GameObjectIsNamed(other, "platform"))
	{
		GameObjectDestroy(gameObject);
	}
	else if (GameObjectIsNamed(other, "Player"))
	{
		GameObjectDestroy(gameObject);
		MonsterSpellCollideHandler();
	}
		
}

void BehaviorMonsterFireBallcloneData(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
	/*MonsterFireBallData * newMonsterFireBallData = calloc(1, sizeof(MonsterFireBallData));
	if (behavior && behavior->dataStructPtr && newMonsterFireBallData)
	{
		*newMonsterFireBallData = *(MonsterFireBallData*)behavior->dataStructPtr;
		behavior->dataStructPtr = (void*)newMonsterFireBallData;
	}*/
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorMonsterFireBallUpdateLifeTimer(BehaviorPtr behavior, float dt)
{
	if (behavior)
	{
		if (behavior->timer > 0)
		{
			behavior->timer -= dt;
			if (behavior->timer <= 0)
				GameObjectDestroy(behavior->parent);
		}
	}
}