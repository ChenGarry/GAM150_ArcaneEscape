//------------------------------------------------------------------------------
//
// File Name:	Slime.c
// Author(s):	Garry Chen, William Fang
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
#include "Slime.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorSlime.h"
#include "ParticleEmitter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 150.0f;
static int flag = 1;
static float time = 0;
static const Vector2D gravityNormal = { 0.0f, -25000.0f };
static const Vector2D gravityNone = { -300.0f, 0.0f };

	// These values are all multiplied by 6
static const Vector2D idleSize = { 136, 100.0f };
static const Vector2D jumpSize = { 152.0f, 108.0f };
static const Vector2D fallSize = { 128.0f, 104.0f };
static const Vector2D hurtSize = { 140.0f, 108.0f };
static const Vector2D moveSize = { 152.0f, 116.0f };

static SlimeState state = SlimeInvalid;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * slimeMesh;
static AEGfxTexture * slimeTexture;
static SpriteSourcePtr  slimeSource;
static BehaviorPtr slimeBehaior;

static AEGfxVertexList * moveMesh;
static AEGfxTexture * moveTexture;
static SpriteSourcePtr  moveSource;

static AEGfxVertexList * hurtMesh;
static AEGfxTexture * hurtTexture;
static SpriteSourcePtr  hurtSource;

static AEGfxVertexList * jumpMesh;
static AEGfxTexture * jumpTexture;
static SpriteSourcePtr  jumpSource;

static AEGfxVertexList * fallMesh;
static AEGfxTexture * fallTexture;
static SpriteSourcePtr  fallSource;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void SlimeLoadAssets(void)
{
	slimeTexture = AEGfxTextureLoad("Assets/slime.png");
	AE_WARNING_MESG(slimeTexture, "Failed to create monster texture!");

	slimeSource = SpriteSourceCreate(1, 1, slimeTexture);

	moveMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 4.0f, 1.0f / 2.0f, "move4x2");
	moveTexture = AEGfxTextureLoad("Assets/SlimeMove.png");
	moveSource = SpriteSourceCreate(4, 2, moveTexture);

	hurtMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "hurt1x1");
	hurtTexture = AEGfxTextureLoad("Assets/SlimeHurt.png");
	hurtSource = SpriteSourceCreate(1, 1, hurtTexture);

	jumpMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f/2.0f, 1.0f/2.0f, "jump2x2");
	jumpTexture = AEGfxTextureLoad("Assets/SlimeJump.png");
	jumpSource = SpriteSourceCreate(2, 2, jumpTexture);

	fallMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "fall1x1");
	fallTexture = AEGfxTextureLoad("Assets/Slimefall.png");
	fallSource = SpriteSourceCreate(1, 1, fallTexture);
}

void SlimeIni()
{
	GameObjectPtr gameObject = GameObjectCreate("Slime");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec;
	Vector2DSet(&tmpVec, 204.0f, 150.0f);
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	ParticleEmitterPtr emitter = ParticleEmitterCreate(gameObject);

	SpritePtr sprite = SpriteCreate("Slime Sprite");
	slimeMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "slime1x1");
	SpriteSetMesh(sprite, slimeMesh);
	SpriteSetSpriteSource(sprite, slimeSource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNormal);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorSlimeCreate();
	ColliderPtr collider = ColliderCreate(gameObject);
	AnimationPtr animation = AnimationCreate(sprite);

	GameObjectAddEmitter(gameObject,emitter);
	GameObjectAddTransform(gameObject, transform);
	GameObjectAddPhysics(gameObject, physics);
	GameObjectAddSprite(gameObject, sprite);
	GameObjectAddBehavior(gameObject, behavior);
	GameObjectAddCollider(gameObject, collider);
	GameObjectAddAnimation(gameObject, animation);

	GameObjectManagerAddArchetype(gameObject);
}

void SlimeUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(gameObeject);
	UNREFERENCED_PARAMETER(dt);
}

void SlimeFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);
}

void SlimeFreeAssets(void)
{
	AEGfxMeshFree(slimeMesh);
	AEGfxTextureUnload(slimeTexture);
	SpriteSourceFree(&slimeSource);
	AEGfxMeshFree(moveMesh);
	AEGfxTextureUnload(moveTexture);
	SpriteSourceFree(&moveSource);
	AEGfxMeshFree(hurtMesh);
	AEGfxTextureUnload(hurtTexture);
	SpriteSourceFree(&hurtSource);
	AEGfxMeshFree(fallMesh);
	AEGfxTextureUnload(fallTexture);
	SpriteSourceFree(&fallSource);
	AEGfxMeshFree(jumpMesh);
	AEGfxTextureUnload(jumpTexture);
	SpriteSourceFree(&jumpSource);
}

SpriteSourcePtr SlimeGetSpriteSource(void)
{
	return slimeSource;
}

void SlimeSetState(GameObjectPtr gameObject, SlimeState newState)
{
	if (state != newState)
	{
		state = newState;
		SpritePtr sprite = GameObjectGetSprite(gameObject);
		AnimationPtr animation = GameObjectGetAnimation(gameObject);
		TransformPtr transform = GameObjectGetTransform(gameObject);
		Vector2D scale = *TransformGetScale(transform);
		switch (newState)
		{
		case SlimeIdle:
			scale.x = idleSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, slimeMesh);
			SpriteSetSpriteSource(sprite, slimeSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;
		case SlimeJump:
			scale.x = jumpSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, jumpMesh);
			SpriteSetSpriteSource(sprite, jumpSource);
			AnimationPlay(animation, 4, 0.125f, false);
			break;
		case SlimeFall:
			scale.x = fallSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, fallMesh);
			SpriteSetSpriteSource(sprite, fallSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;
		case SlimeHurt:
			scale.x = hurtSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, hurtMesh);
			SpriteSetSpriteSource(sprite, hurtSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;
		case SlimeMove:
			scale.x = moveSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, moveMesh);
			SpriteSetSpriteSource(sprite, moveSource);
			AnimationPlay(animation, 8, 0.1f, true);
			break;
		}
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------





