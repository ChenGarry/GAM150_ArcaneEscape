//------------------------------------------------------------------------------
//
// File Name:	MonsterManager.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MonsterManager.h"
#include "MonsterTypeTable.h"
#include "Trace.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Collision.h"
#include "Platforms.h"
#include "IceSpell.h"
#include "FireSpell.h"
#include "elecSpell.h"
#define _USE_MATH_DEFINES // for C
#include <math.h>

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const char* monsterTypeName;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static MonsterTypes *monsterTypeList;
static int maxCount;
static int currentCount;
static int monsterTypeCount;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void MonsterManagerSpawn(float x, float y,float rotation, const char *name);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the monster manager.

void MonsterManagerLoad(MonsterTypes *monsterTypes, int typeCount, int maxMonsterCount)
{
	int i;
	TraceMessage("MM: Load");
	maxCount = maxMonsterCount;

	monsterTypeCount = typeCount;
	monsterTypeList = calloc(typeCount, sizeof(MonsterTypes));
	for (i = 0; i < monsterTypeCount; ++i)
	{
		monsterTypeList[i] = monsterTypes[i];
		MonsterTypeExecuteLoad(monsterTypes[i]);
	}
}

void MonsterManagerInit()
{
	TraceMessage("MM: Init");
	int i;
	for (i = 0; i < monsterTypeCount; ++i)
	{
		MonsterTypeExecuteInit(monsterTypeList[i]);
	}

	currentCount = 0;
}

bool MonsterManagerAddMonster(float x, float y,float rotation, const char *name)
{
	if(currentCount < maxCount)
	{
		++currentCount;
		MonsterManagerSpawn(x, y, rotation, name);
		return true;
	}
	return false;
}

void MonsterManagerDeleteMonster(void)
{
	if(currentCount > 0)
		--currentCount;
}

// Shutdown the monster manager.
void MonsterManagerShutdown()
{
	//int i;
	TraceMessage("MM: Shutdown");

	/*for(i = 0; i < maxCount; i++)
		MonsterFree(&monsterList[i]);*/
}

void MonsterManagerUnload()
{
	TraceMessage("MM: Unload");

	for (int i = 0; i < monsterTypeCount; i++)
		MonsterTypeExecuteUnload(monsterTypeList[i]);
}

int MonsterManagerGetMaxMonsters()
{
	return maxCount;
}

int MonsterManagerGetCurrentCount()
{
	return currentCount;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

static void MonsterManagerSpawn(float x, float y,float rotation, const char *name)
{
	GameObjectPtr archetype = GameObjectManagerGetArchetype(name);

	// Check for a valid archetype pointer.
	if (archetype)
	{
		// Clone the bullet archetype.
		GameObjectPtr clone = GameObjectClone(archetype);

		if (clone)
		{
			TransformPtr cloneTransform = GameObjectGetTransform(clone);

			// Set the Monster's position.
			Vector2D translation;
			Vector2DSet(&translation, x, y);
			TransformSetTranslation(cloneTransform, &translation);
			TransformSetRotation(cloneTransform, rotation*(float)(M_PI/180));
			// Add the cloned object to the game object manager.
			GameObjectManagerAdd(clone);
		}
	}
}