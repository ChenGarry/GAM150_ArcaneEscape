//------------------------------------------------------------------------------
//
// File Name:	Knight.c
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
#include "Knight.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorKnight.h"
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

static const Vector2D knightSize = { -55.0f * 2, 71.0f * 2};
static const Vector2D moveSize = { -55.0f * 2, 74.0f * 2 };
static const Vector2D dieSize = { -55.0f * 2, 71.0f * 2 };

static KnightState state = KnightInvalid;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * KnightMesh;
static AEGfxTexture * KnightTexture;
static SpriteSourcePtr  KnightSource;
static BehaviorPtr KnightBehaior;

static AEGfxVertexList * moveMesh;
static AEGfxTexture * moveTexture;
static SpriteSourcePtr  moveSource;

static AEGfxVertexList * dieMesh;
static AEGfxTexture * dieTexture;
static SpriteSourcePtr  dieSource;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void KnightLoadAssets(void)
{
	KnightTexture = AEGfxTextureLoad("Assets/KnightIdle.png");
	AE_WARNING_MESG(KnightTexture, "Failed to create monster texture!");

	KnightSource = SpriteSourceCreate(1, 1, KnightTexture);

	moveMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 2.0f, 1.0f / 2.0f, "move2x2");
	moveTexture = AEGfxTextureLoad("Assets/KnightMove.png");
	moveSource = SpriteSourceCreate(2, 2, moveTexture);

	dieMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "hurt1x1");
	dieTexture = AEGfxTextureLoad("Assets/KnightHurt.png");
	dieSource = SpriteSourceCreate(1, 1, dieTexture);
}

void KnightIni()
{
	GameObjectPtr gameObject = GameObjectCreate("Knight");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec = knightSize;
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	ParticleEmitterPtr emitter = ParticleEmitterCreate(gameObject);

	SpritePtr sprite = SpriteCreate("Knight Sprite");
	KnightMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Knight1x1");
	SpriteSetMesh(sprite, KnightMesh);
	SpriteSetSpriteSource(sprite, KnightSource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNormal);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorKnightCreate();
	ColliderPtr collider = ColliderCreate(gameObject);
	AnimationPtr animation = AnimationCreate(sprite);

	GameObjectAddEmitter(gameObject, emitter);
	GameObjectAddTransform(gameObject, transform);
	GameObjectAddPhysics(gameObject, physics);
	GameObjectAddSprite(gameObject, sprite);
	GameObjectAddBehavior(gameObject, behavior);
	GameObjectAddCollider(gameObject, collider);
	GameObjectAddAnimation(gameObject, animation);

	GameObjectManagerAddArchetype(gameObject);
}

void KnightUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(gameObeject);
	UNREFERENCED_PARAMETER(dt);
}

void KnightFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);
}

void KnightFreeAssets(void)
{
	AEGfxMeshFree(KnightMesh);
	AEGfxTextureUnload(KnightTexture);
	SpriteSourceFree(&KnightSource);
}

SpriteSourcePtr KnightGetSpriteSource(void)
{
	return KnightSource;
}

void KnightSetState(GameObjectPtr gameObject, KnightState newState)
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
		case KnightIdle:
			scale.x = knightSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, KnightMesh);
			SpriteSetSpriteSource(sprite, KnightSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;
		case KnightMove:
			scale.x = moveSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, moveMesh);
			SpriteSetSpriteSource(sprite, moveSource);
			AnimationPlay(animation, 4, 0.1f, true);
			break;
		case KnightDie:
			scale.x = dieSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, dieMesh);
			SpriteSetSpriteSource(sprite, dieSource);
			AnimationPlay(animation, 6, 0.1f, false);
			break;
		}
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------