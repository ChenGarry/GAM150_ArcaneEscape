//------------------------------------------------------------------------------
//
// File Name:	MonsterFireBall.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright © 2019 DigiPen (USA) Corporation.
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
#include "MonsterFireBall.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorMonsterFireBall.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 150.0f;
static int flag = 1;
static float time = 0;
static const Vector2D gravityNormal = { 0.0f, -25000.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D MonsterFireBallSize = { 15.0f, 15.0f };
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * MonsterFireBallMesh;
static AEGfxTexture * MonsterFireBallTexture;
static SpriteSourcePtr  MonsterFireBallSource;
static BehaviorPtr MonsterFireBallBehaior;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void MonsterFireBallLoadAssets(void)
{
	MonsterFireBallTexture = AEGfxTextureLoad("Assets/MonsterFireBall.png");
	AE_WARNING_MESG(MonsterFireBallTexture, "Failed to create monster texture!");

	MonsterFireBallSource = SpriteSourceCreate(1, 1, MonsterFireBallTexture);
}

void MonsterFireBallIni()
{
	GameObjectPtr gameObject = GameObjectCreate("MonsterFireBall");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec = MonsterFireBallSize;
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	SpritePtr sprite = SpriteCreate("MonsterFireBall Sprite");
	MonsterFireBallMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "MonsterFireBall1x1");
	SpriteSetMesh(sprite, MonsterFireBallMesh);
	SpriteSetSpriteSource(sprite, MonsterFireBallSource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNone);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorMonsterFireBallCreate();
	ColliderPtr collider = ColliderCreate(gameObject);

	GameObjectAddTransform(gameObject, transform);
	GameObjectAddPhysics(gameObject, physics);
	GameObjectAddSprite(gameObject, sprite);
	GameObjectAddBehavior(gameObject, behavior);
	GameObjectAddCollider(gameObject, collider);

	GameObjectManagerAddArchetype(gameObject);
}



void MonsterFireBallUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(gameObeject);
	UNREFERENCED_PARAMETER(dt);
}

void MonsterFireBallFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);
}

void MonsterFireBallFreeAssets(void)
{
	AEGfxMeshFree(MonsterFireBallMesh);
	AEGfxTextureUnload(MonsterFireBallTexture);
	SpriteSourceFree(&MonsterFireBallSource);
}

SpriteSourcePtr MonsterFireBallGetSpriteSource(void)
{
	return MonsterFireBallSource;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------





