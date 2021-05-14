//------------------------------------------------------------------------------
//
// File Name:	Monster.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright � 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Trace.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Monster.h"
#include "MonsterTypeTable.h"
#include "Behavior.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct Monster
{
	MonsterTypes monsterType;

	int monsterId;

	Vector2D monsterSize;

	bool isFrozen;

	float FrozenTimer;

	int IceBlockId;

	void * typeStructPtr;

	GameObjectPtr gameObject;

} Monster;


//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static const Vector2D gravityNormal = { 0.0f, -3200.0f };
static AEGfxVertexList * monsterMesh;
static SpriteSourcePtr  monsterSource;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static SpritePtr MonsterSetSprite(const char* name);
static PhysicsPtr MonsterSetPhysics(void);
static TransformPtr MonsterSetTransform(MonsterPtr monsterPtr, float x, float y);
static BehaviorPtr MonsterSetBehavior(MonsterTypes monsterType);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void MonsterLoadAssets(void)
{
	monsterMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "monster1x1");
}

MonsterPtr MonsterCreate(float x, 
						 float y, 
						 MonsterTypes type, 
						 int id,
						 const Vector2D * size,
						 const char *name)
{
	MonsterPtr monsterPtr = calloc(1, sizeof(Monster));
	monsterPtr->gameObject =  GameObjectCreate(name);
	monsterPtr->monsterType = type;
	monsterPtr->monsterId = id;
	monsterPtr->monsterSize = *size;
	monsterPtr->isFrozen = false;
	monsterPtr->FrozenTimer = MONSTER_FROZEN_TIME;
	monsterPtr->IceBlockId = -1;
	//MonsterTypeExecuteInit(monsterPtr);
	monsterSource = MonsterTypeExecuteGetSpriteSource(type);
	GameObjectAddPhysics(monsterPtr->gameObject, MonsterSetPhysics());
	GameObjectAddSprite(monsterPtr->gameObject, MonsterSetSprite("Monster sprite1"));
	GameObjectAddTransform(monsterPtr->gameObject, MonsterSetTransform(monsterPtr, x, y));
	GameObjectAddBehavior(monsterPtr->gameObject, MonsterSetBehavior(type));

	GameObjectManagerAdd(monsterPtr->gameObject);

	return monsterPtr;
}

void MonsterUpdate(GameObjectPtr playerPtr, MonsterPtr monsterPtr, float dt)
{
	if (monsterPtr)
	{
		MonsterTypeExecuteUpdate(playerPtr, monsterPtr, dt);
		GameObjectUpdate(monsterPtr->gameObject, dt);
	}
}

void MonsterDraw(MonsterPtr monsterPtr)
{
	if (monsterPtr)
	{
		GameObjectDraw(monsterPtr->gameObject);
	}
}

void MonsterFree(MonsterPtr *monsterPtr)
{
	if (*monsterPtr)
	{
		GameObjectFree(&(*monsterPtr)->gameObject);
		free(*monsterPtr);
		*monsterPtr = NULL;
	}
}

void MonsterFreeAssets(void)
{
	AEGfxMeshFree(monsterMesh);
}

GameObjectPtr MonsterGetGameObj(MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return monsterPtr->gameObject;
	else
		return NULL;
}

int MonsterGetId(const MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return monsterPtr->monsterId;
	else
		return 0;
}

MonsterTypes MonsterGetType(const MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return monsterPtr->monsterType;
	else
		return mtNone;
}

const Vector2D * MonsterGetSize(const MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return &monsterPtr->monsterSize;
	else
		return NULL;
}

void ** MonsterGetTypeStrcut(const MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return &monsterPtr->typeStructPtr;
	else
		return NULL;
}

bool MonsterGetIsFrozen(const MonsterPtr monsterPtr) 
{
	if (monsterPtr)
		return monsterPtr->isFrozen;
	else
		return false;
}

void MonsterSetIsFrozen(MonsterPtr monsterPtr, bool frozenState)
{
	if (monsterPtr)
		monsterPtr->isFrozen = frozenState;
		
}

float MonsterGetFrozenTimer(MonsterPtr monsterPtr) 
{
	if (monsterPtr)
		return monsterPtr->FrozenTimer;
	else
		return 0;
}

void MonsterSetFrozenTimer(MonsterPtr monsterPtr, float time)
{
	if (monsterPtr)
		monsterPtr->FrozenTimer = time;
}

int MonsterGetIceBlockID(MonsterPtr monsterPtr)
{
	if (monsterPtr)
		return monsterPtr->IceBlockId;
	else
		return 0;
}

void MonsterSetIceBlockID(MonsterPtr monsterPtr, int id)
{
	if (monsterPtr)
		monsterPtr->IceBlockId = id;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static SpritePtr MonsterSetSprite(const char* name)
{
	SpritePtr newMonsterSprite = SpriteCreate(name);

	SpriteSetMesh(newMonsterSprite, monsterMesh);
	SpriteSetSpriteSource(newMonsterSprite, monsterSource);

	return newMonsterSprite;
}

static PhysicsPtr MonsterSetPhysics(void)
{
	PhysicsPtr newMonsterPhysics = PhysicsCreate();

	PhysicsSetAcceleration(newMonsterPhysics, &gravityNormal);

	return newMonsterPhysics;
}

static TransformPtr MonsterSetTransform(MonsterPtr monsterPtr, float x, float y)
{
	TransformPtr newMonsterTransform = TransformCreate(x, y);

	TransformSetScale(newMonsterTransform, &monsterPtr->monsterSize);

	return newMonsterTransform;
}

static BehaviorPtr MonsterSetBehavior(MonsterTypes monsterType)
{
	UNREFERENCED_PARAMETER(monsterType);
	return NULL;
}



