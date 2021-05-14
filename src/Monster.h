//------------------------------------------------------------------------------
//
// File Name:	Monster.h
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
#include "MonsterTypeTable.h"

#define MONSTER_FROZEN_TIME 200

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
typedef struct Monster * MonsterPtr;

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
void MonsterLoadAssets(void);

MonsterPtr MonsterCreate(float x,
						 float y,
						 MonsterTypes type,
						 int id,
						 const Vector2D * size,
						 const char *name);

void MonsterUpdate(GameObjectPtr playerPtr, MonsterPtr monsterPtr, float dt);

void MonsterDraw(MonsterPtr monsterPtr);

void MonsterFree(MonsterPtr *monsterPtr);

void MonsterFreeAssets(void);

GameObjectPtr MonsterGetGameObj(MonsterPtr monsterPtr);

int MonsterGetId(const MonsterPtr monsterPtr);

MonsterTypes MonsterGetType(const MonsterPtr monsterPtr);

const Vector2D * MonsterGetSize(const MonsterPtr monsterPtr);

void ** MonsterGetTypeStrcut(const MonsterPtr monsterPtr);

bool MonsterGetIsFrozen(const MonsterPtr monsterPtr);

void MonsterSetIsFrozen(MonsterPtr monsterPtr, bool frozenState);

float MonsterGetFrozenTimer(MonsterPtr monsterPtr);

void MonsterSetFrozenTimer(MonsterPtr monsterPtr, float time);

int MonsterGetIceBlockID(MonsterPtr monsterPtr);

void MonsterSetIceBlockID(MonsterPtr monsterPtr, int id);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

