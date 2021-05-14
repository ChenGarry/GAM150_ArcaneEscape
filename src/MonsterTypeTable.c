//------------------------------------------------------------------------------
//
// File Name:	MonsterTypeTable.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "MonsterTypeTable.h"
#include "Trace.h"
#include "GameObject.h"
/* Include all monster type headers here. */
#include "Slime.h"
#include "Knight.h"
#include "Floater.h"
#include "Flower.h"
#include "MonsterFireBall.h"
#include "Dummy.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef void(*MonsterTypeVoidFunctionPtr)(void);
typedef void(*MonsterTypeMPFunctionPtr)(GameObjectPtr monsterPtr);
typedef void(*MonsterTypeDtFunctionPtr)(GameObjectPtr monsterPtr, float dt);
typedef SpriteSourcePtr (*MonsterTypeSpriteMTFunctionPtr)(void);
typedef struct
{
	const char *				monsterTypeName;		/* The name of the monster type as a zero-terminated string. */
	MonsterTypeVoidFunctionPtr	monsterTypeLoad;		/* Pointer to a monster type load function.					 */
	MonsterTypeVoidFunctionPtr	monsterTypeInit;		/* Pointer to a monster type initialization function.		 */
	MonsterTypeDtFunctionPtr	monsterTypeUpdate;		/* Pointer to a monster type update function.				 */
	MonsterTypeMPFunctionPtr	monsterTypeShutdown;	/* Pointer to a monster type shutdown function.				 */
	MonsterTypeVoidFunctionPtr	monsterTypeUnload;		/* Pointer to a monster type unload function.				 */
	MonsterTypeSpriteMTFunctionPtr monsterTypeGetSprite;

} MonsterTypeTableEntry;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static const MonsterTypeTableEntry MonsterTypeTab[mtNum] =
{
	{ "Slime",	SlimeLoadAssets, SlimeIni, SlimeUpdate, SlimeFree, SlimeFreeAssets, SlimeGetSpriteSource},
	{ "Knight",	KnightLoadAssets, KnightIni, KnightUpdate, KnightFree, KnightFreeAssets, KnightGetSpriteSource},
	{ "Floater", FloaterLoadAssets, FloaterIni, FloaterUpdate, FloaterFree, FloaterFreeAssets, FloaterGetSpriteSource },
	{ "Flower",	FlowerLoadAssets, FlowerIni, FlowerUpdate, FlowerFree, FlowerFreeAssets, FlowerGetSpriteSource},
	{ "MonsterFireBall", MonsterFireBallLoadAssets, MonsterFireBallIni, MonsterFireBallUpdate, MonsterFireBallFree, MonsterFireBallFreeAssets, MonsterFireBallGetSpriteSource},
	{ "Dummy", DummyLoadAssets, DummyIni, DummyUpdate, DummyFree, DummyFreeAssets, DummyGetSpriteSource},
	// TODO: Add new monster types here.
};

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Determine if the monster type is valid.
bool MonsterTypeIsValid(MonsterTypes monsterType)
{
	return ((0 <= monsterType) && (monsterType < mtNum));
}

// Determine if the monster type is a "special" monster type.
bool MonsterTypeIsSpecial(MonsterTypes monsterType)
{
	return ((monsterType == mtNone));
}

// Execute the Load function for the current monster type.
void MonsterTypeExecuteLoad(MonsterTypes monsterType)
{
	// First validate the monster type and the function pointer.
	if (MonsterTypeIsValid(monsterType) &&
		((*MonsterTypeTab[monsterType].monsterTypeLoad) != NULL))
	{
		// Calls TraceMessage, passing the format string "%s: Load" and the name of the monster type.
		TraceMessage("%s: Load", MonsterTypeTab[monsterType].monsterTypeName);
		// Execute the Init function.
		(*MonsterTypeTab[monsterType].monsterTypeLoad)();
	}
}

// Execute the Init function for the current monster type.
void MonsterTypeExecuteInit(MonsterTypes monsterType)
{
	// First validate the monster type and the function pointer.
	if (MonsterTypeIsValid(monsterType) &&
		((*MonsterTypeTab[monsterType].monsterTypeInit) != NULL))
	{
		// Calls TraceMessage, passing the format string "%s: Init" and the name of the monster type.
		TraceMessage("%s: Init", MonsterTypeTab[monsterType].monsterTypeName);
		// Execute the Init function.
		(*MonsterTypeTab[monsterType].monsterTypeInit)();
	}
}

// Execute the Update function for the current monster type.
void MonsterTypeExecuteUpdate(GameObjectPtr monsterPtr, float dt)
{
	UNREFERENCED_PARAMETER(monsterPtr);
	UNREFERENCED_PARAMETER(dt);
	/*if (monsterPtr) 
	{
		MonsterTypes monsterType = MonsterGetType(monsterPtr);
		// First validate the monster type and the function pointer.
		if (MonsterTypeIsValid(monsterType) &&
			((*MonsterTypeTab[monsterType].monsterTypeUpdate) != NULL))
		{
			// Calls TraceMessage, passing the format string "%s: Update" and the name of the monster type.
			TraceMessage("%s: Update", MonsterTypeTab[monsterType].monsterTypeName);
			// Execute the Update function.
			(*MonsterTypeTab[monsterType].monsterTypeUpdate)(monsterPtr, dt);
		}
	}*/
}

// Execute the Shutdown function for the current monster type.
void MonsterTypeExecuteShutdown(GameObjectPtr monsterPtr)
{
	UNREFERENCED_PARAMETER(monsterPtr);
	/*if (monsterPtr)
	{
		MonsterTypes monsterType = MonsterGetType(monsterPtr);
		// First validate the monster type and the function pointer.
		if (MonsterTypeIsValid(monsterType) &&
			((*MonsterTypeTab[monsterType].monsterTypeShutdown) != NULL))
		{
			// Calls TraceMessage, passing the format string "%s: Shutdown" and the name of the monster type.
			TraceMessage("%s: Shutdown", MonsterTypeTab[monsterType].monsterTypeName);
			// Execute the Shutdown function.
			(*MonsterTypeTab[monsterType].monsterTypeShutdown)(monsterPtr);
		}
	}*/
}

// Execute the Unload function for the current monster type.
void MonsterTypeExecuteUnload(MonsterTypes monsterType)
{
	// First validate the monster type and the function pointer.
	if (MonsterTypeIsValid(monsterType) &&
		((*MonsterTypeTab[monsterType].monsterTypeUnload) != NULL))
	{
		// Calls TraceMessage, passing the format string "%s: Unload" and the name of the monster type.
		TraceMessage("%s: Unload", MonsterTypeTab[monsterType].monsterTypeName);
		// Execute the Init function.
		(*MonsterTypeTab[monsterType].monsterTypeUnload)();
	}
}

SpriteSourcePtr MonsterTypeExecuteGetSpriteSource(MonsterTypes monsterType)
{
	// First validate the monster type and the function pointer.
	if (MonsterTypeIsValid(monsterType) &&
		((*MonsterTypeTab[monsterType].monsterTypeUnload) != NULL))
	{
		// Calls TraceMessage, passing the format string "%s: Unload" and the name of the monster type.
		TraceMessage("%s: Get Sprite Source", MonsterTypeTab[monsterType].monsterTypeName);
		// Execute the Init function.
		return (*MonsterTypeTab[monsterType].monsterTypeGetSprite)();
	}

	return NULL;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

