//------------------------------------------------------------------------------
//
// File Name:	MonsterTypeTable.h
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
#include "SpriteSource.h"
#include "GameObject.h"
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

typedef enum 
{
	/* Special Game States */
	mtNone = -1,

	/* Normal Game States: */
	mtSlime,
	mtKnight,
	mtFloater,
	mtFlower,
	mtMonsterFireBall,
	mtFloat,
	mtDummy,
	// TODO: Add new game states here.

	/* Bookkeeping States: */
	mtNum,
	mtLast = mtNum - 1,

} MonsterTypes;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
typedef struct Monster * MonsterPtr;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Determine if the game state is valid.
bool MonsterTypeIsValid(MonsterTypes monsterType);

// Determine if the game state is a "special" game state.
bool MonsterTypeIsSpecial(MonsterTypes monsterType);

// Execute the Load function for the current game state.
void MonsterTypeExecuteLoad(MonsterTypes monsterType);

// Execute the Init function for the current game state.
void MonsterTypeExecuteInit(MonsterTypes monsterType);

// Execute the Update function for the current game state.
void MonsterTypeExecuteUpdate(GameObjectPtr monsterPtr, float dt);

// Execute the Shutdown function for the current game state.
void MonsterTypeExecuteShutdown(GameObjectPtr monsterPtr);

// Execute the Unload function for the current game state.
void MonsterTypeExecuteUnload(MonsterTypes monsterType);

SpriteSourcePtr MonsterTypeExecuteGetSpriteSource(MonsterTypes monsterType);
//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

