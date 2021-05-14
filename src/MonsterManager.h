//------------------------------------------------------------------------------
//
// File Name:	MonsterManager.h
// Author(s):	Garry Chen
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
#include "MonsterTypeTable.h"
#include "Vector2D.h"
#include "GameObject.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

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

// Initialize the monster manager.
void MonsterManagerLoad(MonsterTypes *monsterTypes, int typeCount, int maxMonsterCount);

void MonsterManagerInit();

bool MonsterManagerAddMonster(float x, float y,float degrees, const char *name);

// Shutdown the monster manager.
void MonsterManagerShutdown();

void MonsterManagerUnload();

int MonsterManagerGetMaxMonsters();

int MonsterManagerGetCurrentCount();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

