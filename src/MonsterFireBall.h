//------------------------------------------------------------------------------
//
// File Name:	MonsterFireBall.h
// Author(s):	Garry Chen
// Project:		MyGame
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
typedef struct MonsterFireBall * MonsterFireBallPtr;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void MonsterFireBallLoadAssets(void);

void MonsterFireBallIni(void);

void MonsterFireBallUpdate(GameObjectPtr gameObeject, float dt);

void MonsterFireBallFree(GameObjectPtr gameObeject);

void MonsterFireBallFreeAssets(void);

SpriteSourcePtr MonsterFireBallGetSpriteSource(void);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

