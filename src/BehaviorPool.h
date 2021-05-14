//------------------------------------------------------------------------------
//
// File Name:	BehaviorPool.h
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Vector2D.h"
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
typedef struct GameObject * GameObjectPtr;
typedef struct Behavior * BehaviorPtr;
typedef struct SpriteSource * SpriteSourcePtr;
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void BehaviorPoolSeekOnGround(GameObjectPtr gameObject, const Vector2D *targetPos, float speed);

void BehaviorPoolSeekInAir(GameObjectPtr gameObject, const Vector2D *targetPos, float speed);

void BehaviorPoolMove(GameObjectPtr gameObject, const Vector2D *moveVelocity);

void BehaviorPoolPatrol(GameObjectPtr gameObject, GameObjectPtr platform);

void BehaviorPoolChangeSide(GameObjectPtr gameObject);

void BehaviorPoolStop(GameObjectPtr gameObject);

void BehaviorPoolSetXVolicity(GameObjectPtr gameObject, float x);

void BehaviorPoolSetYVolicity(GameObjectPtr gameObject, float y);

void BehaviorPoolSetXAcceleration(GameObjectPtr gameObject, float x);

void BehaviorPoolSetYAcceleration(GameObjectPtr gameObject, float y);

void BehaviorPoolJump(GameObjectPtr gameObject, const Vector2D *jumpVelocity, const Vector2D *gravity);

void BehaviorPoolShoot(GameObjectPtr gameObject, char* projectileName, const Vector2D *targetPos, float speed);

void BehaviorPoolMagnetizePoint(GameObjectPtr gameObect, GameObjectPtr* orbs, int size);

void BehaviorPoolSetFrozen(GameObjectPtr gameObect, SpriteSourcePtr iceBlockSource);

void BehaviorPoolSetFire(GameObjectPtr gameObect);

void BehaviorPoolSetMagnetize(GameObjectPtr gameObect);

void BehaviorPoolOnFireTimer(GameObjectPtr gameObect, float dt);

void BehaviorPoolUnfreezeTimer(GameObjectPtr gameObect, float dt);

void BehaviorPoolUpdateTimer(BehaviorPtr behavior, float dt);

void BehaviorPoolUpdateMagTimer(BehaviorPtr behavior, float dt);

void BehaviorPoolCheckAggro(GameObjectPtr gameObect, GameObjectPtr player);
//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

