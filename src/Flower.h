//------------------------------------------------------------------------------
//
// File Name:	Flower.h
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
typedef struct Flower * FlowerPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------
enum FlowerState
{
	FlowerInvalid = -1,
	FlowerIdle,
	FlowerHurt,
	FlowerDie
} typedef FlowerState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void FlowerLoadAssets(void);

void FlowerIni(void);

void FlowerUpdate(GameObjectPtr gameObeject, float dt);

void FlowerFree(GameObjectPtr gameObeject);

void FlowerFreeAssets(void);

SpriteSourcePtr FlowerGetSpriteSource(void);

void FlowerSetState(GameObjectPtr gameObject, FlowerState newState);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

