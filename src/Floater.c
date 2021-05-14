//------------------------------------------------------------------------------
//
// File Name:	Floater.c
// Author(s):	Lorenzo DeMaine, Garry Chen
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
#include "Floater.h"
#include "PlayerCharacter.h"
#include "Behavior.h"
#include "BehaviorFloater.h"
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
static const Vector2D FloaterSize = { 36.0f * 2, 35.0f * 2 };

static const Vector2D moveSize = { 56.0f * 2, 30.0f * 2 };
static const Vector2D hurtSize = { 43.0f * 2, 33.0f * 2 };
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList * FloaterMesh;
static AEGfxTexture * FloaterTexture;
static SpriteSourcePtr  FloaterSource;
static BehaviorPtr FloaterBehaior;

static AEGfxVertexList * moveMesh;
static AEGfxTexture * moveTexture;
static SpriteSourcePtr  moveSource;

static AEGfxVertexList * hurtMesh;
static AEGfxTexture * hurtTexture;
static SpriteSourcePtr  hurtSource;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void FloaterLoadAssets(void)
{
	FloaterTexture = AEGfxTextureLoad("Assets/GhostIdle.png");
	AE_WARNING_MESG(FloaterTexture, "Failed to create monster texture!");

	FloaterSource = SpriteSourceCreate(1, 1, FloaterTexture);

	moveMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3.0f, 1.0f / 2.0f, "move3x2");
	moveTexture = AEGfxTextureLoad("Assets/GhostMove.png");
	moveSource = SpriteSourceCreate(3, 2, moveTexture);

	hurtMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "hurt1x1");
	hurtTexture = AEGfxTextureLoad("Assets/GhostHurt.png");
	hurtSource = SpriteSourceCreate(1, 1, hurtTexture);
}

void FloaterIni()
{
	GameObjectPtr gameObject = GameObjectCreate("Floater");

	TransformPtr transform = TransformCreate(0.0f, 0.0f);
	TransformSetRotation(transform, 0.0f);
	Vector2D tmpVec = FloaterSize;
	TransformSetScale(transform, &tmpVec);
	TransformSetHitboxScale(transform, &tmpVec);

	ParticleEmitterPtr emitter = ParticleEmitterCreate(gameObject);

	SpritePtr sprite = SpriteCreate("Floater Sprite");
	FloaterMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Floater1x1");
	SpriteSetMesh(sprite, FloaterMesh);
	SpriteSetSpriteSource(sprite, FloaterSource);

	PhysicsPtr physics = PhysicsCreate();
	PhysicsSetAcceleration(physics, &gravityNormal);
	PhysicsSetVelocity(physics, &gravityNone);

	BehaviorPtr behavior = BehaviorFloaterCreate();
	//ColliderPtr collider = ColliderCreate(gameObject);
	AnimationPtr animation = AnimationCreate(sprite);

	GameObjectAddEmitter(gameObject, emitter);
	GameObjectAddTransform(gameObject, transform);
	GameObjectAddPhysics(gameObject, physics);
	GameObjectAddSprite(gameObject, sprite);
	GameObjectAddBehavior(gameObject, behavior);
	//GameObjectAddCollider(gameObject, collider);
	GameObjectAddAnimation(gameObject, animation);

	GameObjectManagerAddArchetype(gameObject);
}

void FloaterUpdate(GameObjectPtr gameObeject, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	UNREFERENCED_PARAMETER(gameObeject);
}

void FloaterFree(GameObjectPtr gameObeject)
{
	UNREFERENCED_PARAMETER(gameObeject);

}

void FloaterFreeAssets(void)
{
	AEGfxMeshFree(FloaterMesh);
	AEGfxTextureUnload(FloaterTexture);
	SpriteSourceFree(&FloaterSource);
	AEGfxMeshFree(moveMesh);
	AEGfxTextureUnload(moveTexture);
	SpriteSourceFree(&moveSource);
	AEGfxMeshFree(hurtMesh);
	AEGfxTextureUnload(hurtTexture);
	SpriteSourceFree(&hurtSource);
}

SpriteSourcePtr FloaterGetSpriteSource(void)
{
	return FloaterSource;
}

void FloaterSetState(GameObjectPtr gameObject, FloaterState newState)
{
	SpritePtr sprite = GameObjectGetSprite(gameObject);
	AnimationPtr animation = GameObjectGetAnimation(gameObject);
	TransformPtr transform = GameObjectGetTransform(gameObject);
	Vector2D scale = *TransformGetScale(transform);
	switch (newState)
	{
	case FloaterIdle:
		scale.x = FloaterSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
		TransformSetScale(transform, &scale);
		SpriteSetMesh(sprite, FloaterMesh);
		SpriteSetSpriteSource(sprite, FloaterSource);
		AnimationPlay(animation, 1, 0.0f, false);
		break;
	case FloaterHurt:
		scale.x = hurtSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
		TransformSetScale(transform, &scale);
		SpriteSetMesh(sprite, hurtMesh);
		SpriteSetSpriteSource(sprite, hurtSource);
		AnimationPlay(animation, 1, 0.0f, false);
		break;
	case FloaterMove:
		scale.x = moveSize.x* (TransformGetScale(transform)->x / fabsf(TransformGetScale(transform)->x));
		TransformSetScale(transform, &scale);
		SpriteSetMesh(sprite, moveMesh);
		SpriteSetSpriteSource(sprite, moveSource);
		AnimationPlay(animation, 6, 0.1f, true);
		break;
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------






