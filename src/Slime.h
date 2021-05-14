//------------------------------------------------------------------------------
//
// File Name:	Slime.h
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
typedef struct Slime * SlimePtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------
enum SlimeState
{
	SlimeInvalid = -1,
	SlimeIdle,
	SlimeMove,
	SlimeJump,
	SlimeFall,
	SlimeHurt
} typedef SlimeState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void SlimeLoadAssets(void);

void SlimeIni(void);

void SlimeUpdate(GameObjectPtr gameObeject, float dt);

void SlimeFree(GameObjectPtr gameObeject);

void SlimeFreeAssets(void);

void SlimeSetState(GameObjectPtr gameObject, SlimeState newState);

SpriteSourcePtr SlimeGetSpriteSource(void);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

