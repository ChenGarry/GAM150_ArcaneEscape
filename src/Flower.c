//------------------------------------------------------------------------------
//
// File Name:	Flower.c
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
#include "Flower.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorFlower.h"
#include "ParticleEmitter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float groundHeight = -150.0f;
static const float moveVelocity = 150.0f;
static int flag = 1;
static float time = 0;
static const Vector2D gravityNormal = { 0.0f, -25000.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D flowerSize = {21.0f * 3, 50.0f * 3};

static const Vector2D hurtSize = { 26.0f * 3, 46.0f * 3 };
static const Vector2D dieSize = { 50.0f * 3, 41.0f * 3};

static FlowerState state = FlowerInvalid;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * FlowerMesh;
static AEGfxTexture * FlowerTexture;
static SpriteSourcePtr  FlowerSource;
static BehaviorPtr FlowerBehaior;

static AEGfxTexture * leftTexture;
static SpriteSourcePtr  leftSource;

static AEGfxVertexList * hurtMesh;
static AEGfxTexture * hurtTexture;
static SpriteSourcePtr  hurtSource;

static AEGfxVertexList * dieMesh;
static AEGfxTexture * dieTexture;
static SpriteSourcePtr  dieSource;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//static bool FlowerIsAggro(const Vector2D *playerTranslation, const Vector2D *monsterTranslation);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void FlowerLoadAssets(void)
{
	FlowerTexture = AEGfxTextureLoad("Assets/Eye.png");
	AE_WARNING_MESG(FlowerTexture, "Failed to create monster texture!");

	FlowerSource = SpriteSourceCreate(4, 2, FlowerTexture);

	leftTexture = AEGfxTextureLoad("Assets/LeftEye.png");
	leftSource = SpriteSourceCreate(4, 2, leftTexture);

	hurtMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "hurt1x1");
	hurtTexture = AEGfxTextureLoad("Assets/EyeHurt.png");
	hurtSource = SpriteSourceCreate(1, 1, hurtTexture);

	dieMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 2.0f, 1.0f, "die2x1");
	dieTexture = AEGfxTextureLoad("Assets/EyeDie.png");
	dieSource = SpriteSourceCreate(1, 1, dieTexture);

}

void FlowerIni()
{
	GameObjectPtr gameObject = GameObjectCreate("Flower");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec = flowerSize;
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	ParticleEmitterPtr emitter = ParticleEmitterCreate(gameObject);

	SpritePtr sprite = SpriteCreate("Flower Sprite");
	FlowerMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f/4.0f, 1.0f/2.0f, "Flower4x2");
	SpriteSetMesh(sprite, FlowerMesh);
	SpriteSetSpriteSource(sprite, FlowerSource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNone);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorFlowerCreate();
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
	AnimationPlay(animation, 7, 0.075f, true);
}

void FlowerUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(gameObeject);	
}

void FlowerFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);
}

void FlowerFreeAssets(void)
{
	AEGfxMeshFree(FlowerMesh);
	AEGfxTextureUnload(FlowerTexture);
	SpriteSourceFree(&FlowerSource);

	AEGfxTextureUnload(leftTexture);
	SpriteSourceFree(&leftSource);
	AEGfxMeshFree(hurtMesh);
	AEGfxTextureUnload(hurtTexture);
	SpriteSourceFree(&hurtSource);
	AEGfxMeshFree(dieMesh);
	AEGfxTextureUnload(dieTexture);
	SpriteSourceFree(&dieSource);
}

SpriteSourcePtr FlowerGetSpriteSource(void)
{
	return FlowerSource;
}

void FlowerSetState(GameObjectPtr gameObject, FlowerState newState)
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
		case FlowerIdle:
			scale.x = flowerSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, FlowerMesh);
			SpriteSetSpriteSource(sprite, FlowerSource);
			AnimationPlay(animation, 7, 0.075f, true);
			break;
		case FlowerHurt:
			scale.x = hurtSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, hurtMesh);
			SpriteSetSpriteSource(sprite, hurtSource);
			AnimationPlay(animation, 1, 0.0f, false);
			break;
		case FlowerDie:
			scale.x = dieSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
			TransformSetScale(transform, &scale);
			SpriteSetMesh(sprite, dieMesh);
			SpriteSetSpriteSource(sprite, dieSource);
			AnimationPlay(animation, 2, 0.1f, false);
			break;
		}
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------




