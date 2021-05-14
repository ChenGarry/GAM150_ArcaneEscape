//------------------------------------------------------------------------------
//
// File Name:	Knight.h
// Author(s):	Garry Chen
// Project:		MyGame
// Course:		GAM150S19-c
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "GameObject.h"
#include "PlayerCharacter.h"
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
typedef struct Knight * KnightPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------
enum KnightState
{
	KnightInvalid = -1,
	KnightIdle,
	KnightMove,
	KnightDie
} typedef KnightState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void KnightLoadAssets(void);

void KnightIni(void);

void KnightUpdate(GameObjectPtr gameObeject, float dt);

void KnightFree(GameObjectPtr gameObeject);

void KnightFreeAssets(void);

SpriteSourcePtr KnightGetSpriteSource(void);

void KnightSetState(GameObjectPtr gameObject, KnightState newState);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

