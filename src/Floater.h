//------------------------------------------------------------------------------
//
// File Name:	Floater.h
// Author(s):	Lorenzo DeMaine, Garry Chen
// Project:		ArcaneEscape
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
typedef struct Floater * FloaterPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------
enum FloaterState
{
	FloaterInvalid = -1,
	FloaterIdle,
	FloaterMove,
	FloaterHurt
} typedef FloaterState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void FloaterLoadAssets(void);

void FloaterIni(void);

void FloaterUpdate(GameObjectPtr gameObeject, float dt);

void FloaterFree(GameObjectPtr gameObeject);

void FloaterFreeAssets(void);

SpriteSourcePtr FloaterGetSpriteSource(void);

void FloaterSetState(GameObjectPtr gameObject, FloaterState newState);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

