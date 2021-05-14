//------------------------------------------------------------------------------
//
// File Name:	Dummy.c
// Author(s):	Lorenzo DeMaine, Garry Chen
// Project:		MyGame
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
#include "Dummy.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorDummy.h"
#include "ParticleEmitter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 150.0f;
static int flag = 1;
static float time = 0;
static const Vector2D gravityNormal = { 0.0f, 0.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D DummySize = { 75.0f, 75.0f };
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * DummyMesh;
static AEGfxTexture * DummyTexture;
static SpriteSourcePtr  DummySource;
static BehaviorPtr DummyBehaior;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void DummyLoadAssets(void)
{
	DummyTexture = AEGfxTextureLoad("Assets/Mannequin.png");
	AE_WARNING_MESG(DummyTexture, "Failed to create monster texture!");

	DummySource = SpriteSourceCreate(1, 1, DummyTexture);
}

void DummyIni()
{
	GameObjectPtr gameObject = GameObjectCreate("Dummy");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec = DummySize;
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	ParticleEmitterPtr emitter = ParticleEmitterCreate(gameObject);

	SpritePtr sprite = SpriteCreate("Dummy Sprite");
	DummyMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Dummy1x1");
	SpriteSetMesh(sprite, DummyMesh);
	SpriteSetSpriteSource(sprite, DummySource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNormal);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorDummyCreate();
	//ColliderPtr collider = ColliderCreate(gameObject);

	GameObjectAddEmitter(gameObject, emitter);
	GameObjectAddTransform(gameObject, transform);
	GameObjectAddPhysics(gameObject, physics);
	GameObjectAddSprite(gameObject, sprite);
	GameObjectAddBehavior(gameObject, behavior);
	//GameObjectAddCollider(gameObject, collider);

	GameObjectManagerAddArchetype(gameObject);
}

void DummyUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(gameObeject);
}

void DummyFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);

}

void DummyFreeAssets(void)
{
	AEGfxMeshFree(DummyMesh);
	AEGfxTextureUnload(DummyTexture);
	SpriteSourceFree(&DummySource);
}

SpriteSourcePtr DummyGetSpriteSource(void)
{
	return DummySource;
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------






