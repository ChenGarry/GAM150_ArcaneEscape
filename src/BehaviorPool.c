//------------------------------------------------------------------------------
//
// File Name:	BehaviorPool.c
// Author(s):	Garry Chen
// Project:		MyGame
//
// Copyright ?2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"
#include "BehaviorPool.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Collision.h"
#include "Transform.h"
#include "IcePlatforms.h"
#include "Physics.h"
#include "Platforms.h"
#include "ParticleEmitter.h"
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
#define MONSTER_FROZEN_TIME 100
#define MONSTER_FIRE_TIME 50

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static const Vector2D gravityNone = { 0.0f, 0.0f }; 
static const Vector2D gravityNormal = { 0.0f, -30000.0f };
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void BehaviorPoolSeekOnGround(GameObjectPtr gameObject, const Vector2D *targetPos, float speed)
{
	if (gameObject && targetPos)
	{
		TransformPtr transform = GameObjectGetTransform(gameObject);
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);

		if (transform && physic)
		{
			Vector2D velocity = *PhysicsGetVelocity(physic);
			Vector2D translation = *TransformGetTranslation(transform);
			int dir;
			if (translation.x > targetPos->x)
				dir = 1;
			else
				dir = -1;
			Vector2DSet(&velocity, speed * dir, 0.0f);
			PhysicsSetVelocity(physic, &velocity);
		}
	}
}

void BehaviorPoolSeekInAir(GameObjectPtr gameObject, const Vector2D *targetPos, float speed)
{
	if (gameObject && targetPos)
	{
		TransformPtr transform = GameObjectGetTransform(gameObject);
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);

		if (transform && physic)
		{
			Vector2D velocity = *PhysicsGetVelocity(physic);
			Vector2D translation = *TransformGetTranslation(transform);
			Vector2D dir;
			Vector2DSub(&dir, targetPos, &translation);
			Vector2DNormalize(&dir, &dir);

			Vector2DScale(&velocity, &velocity, speed);
			PhysicsSetVelocity(physic, &velocity);
		}
	}
}

void BehaviorPoolMove(GameObjectPtr gameObject, const Vector2D *moveVelocity)
{
	if (gameObject && moveVelocity)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		if (physic)
		{
			PhysicsSetVelocity(physic, moveVelocity);
		}
	}
}

void BehaviorPoolPatrol(GameObjectPtr gameObject, GameObjectPtr platform)
{
	if (gameObject && platform)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		TransformPtr gameObjectTransform = GameObjectGetTransform(gameObject);
		TransformPtr platformTransform = GameObjectGetTransform(platform);
		if (physic && gameObjectTransform && platformTransform)
		{
			const Vector2D * gameObjectPreviousPosition = PhysicsGetOldTranslation(physic);
			const Vector2D * gameObjectSize = TransformGetScale(gameObjectTransform);
			const Vector2D * gameObjectVelocity = PhysicsGetVelocity(physic);
			const Vector2D * platformPosition = TransformGetTranslation(platformTransform);
			const Vector2D * platformSize = TransformGetScale(platformTransform);
			Vector2D gameObjectNewPosition;
			Vector2DAdd(&gameObjectNewPosition, gameObjectPreviousPosition, gameObjectVelocity);
			BehaviorPtr behavior = GameObjectGetBehavior(gameObject);

			if (gameObjectVelocity->x > 0 &&
				gameObjectPreviousPosition->x + fabsf(gameObjectSize->x / 2) > platformPosition->x + (platformSize->x / 2))
			{
				BehaviorPoolSetXVolicity(gameObject, -gameObjectVelocity->x);
				behavior->isFacingRight = false;
			}
			else if (gameObjectVelocity->x < 0 &&
				gameObjectPreviousPosition->x - fabsf(gameObjectSize->x / 2) < platformPosition->x - (platformSize->x / 2))
			{
				BehaviorPoolSetXVolicity(gameObject, -gameObjectVelocity->x);
				behavior->isFacingRight = true;
			}
		}
	}
}

void BehaviorPoolChangeSide(GameObjectPtr gameObject)
{
	if (gameObject)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObject);
		TransformPtr transform = GameObjectGetTransform(gameObject);
		const Vector2D *scale = TransformGetScale(transform);
		if ((behavior->isFacingRight && (scale->x < 0)) ||
			(!behavior->isFacingRight && (scale->x > 0)))
		{
			Vector2D flip;
			flip.x = -scale->x; flip.y = scale->y;
			TransformSetScale(transform, &flip);
		}
	}
}

void BehaviorPoolStop(GameObjectPtr gameObject)
{
	Vector2D zero = { 0.0f, -0.0f };
	BehaviorPoolMove(gameObject, &zero);
}

void BehaviorPoolSetXVolicity(GameObjectPtr gameObject, float x)
{
	if (gameObject)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		if (physic)
		{
			Vector2D velocity = *PhysicsGetVelocity(physic);
			velocity.x = x;
			PhysicsSetVelocity(physic, &velocity);
		}
	}
}

void BehaviorPoolSetYVolicity(GameObjectPtr gameObject, float y)
{
	if (gameObject)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		if (physic)
		{
			Vector2D velocity = *PhysicsGetVelocity(physic);
			velocity.y = y;
			PhysicsSetVelocity(physic, &velocity);
		}
	}
}

void BehaviorPoolSetXAcceleration(GameObjectPtr gameObject, float x)
{
	if (gameObject)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		if (physic)
		{
			Vector2D acceleration = *PhysicsGetAcceleration(physic);
			acceleration.x = x;
			PhysicsSetAcceleration(physic, &acceleration);
		}
	}
}

void BehaviorPoolSetYAcceleration(GameObjectPtr gameObject, float y)
{
	if (gameObject)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		if (physic)
		{
			Vector2D acceleration = *PhysicsGetAcceleration(physic);
			acceleration.y = y;
			PhysicsSetAcceleration(physic, &acceleration);
		}
	}
}

void BehaviorPoolJump(GameObjectPtr gameObject, const Vector2D *jumpVelocity, const Vector2D *gravity)
{
	if (gameObject && jumpVelocity)
	{
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		BehaviorPtr behavior = GameObjectGetBehavior(gameObject);
		if (physic && behavior)
		{
			behavior->inAir = true;
			PhysicsSetVelocity(physic, jumpVelocity);
			PhysicsSetAcceleration(physic, gravity);
		}
	}
}


void BehaviorPoolShoot(GameObjectPtr gameObject, char* projectileName, const Vector2D *targetPos, float speed)
{
	if (gameObject)
	{
		GameObjectPtr archetype = GameObjectManagerGetArchetype(projectileName);

		// Check for a valid archetype pointer.
		if (archetype)
		{
			// Clone the bullet archetype.
			GameObjectPtr clone = GameObjectClone(archetype);

			// Get the shooter's position.
			TransformPtr transform = GameObjectGetTransform(gameObject);
			PhysicsPtr physic = GameObjectGetPhysics(gameObject);

			if (transform && physic && clone)
			{
				TransformPtr cloneTransform = GameObjectGetTransform(clone);
				PhysicsPtr clonePhysic = GameObjectGetPhysics(clone);

				// Set the bullet's position and rotation.
				Vector2D translation = *TransformGetTranslation(transform);
				Vector2D dir;
				Vector2DSub(&dir, targetPos, &translation);
				Vector2DNormalize(&dir, &dir);

				TransformSetRotation(cloneTransform, Vector2DToAngleRad(&dir));
				TransformSetTranslation(cloneTransform, TransformGetTranslation(transform));

				// Set the bullet's velocity.
				Vector2D velocity;
				Vector2DScale(&velocity, &dir, speed);
				PhysicsSetVelocity(clonePhysic, &velocity);

				// Add the cloned object to the game object manager.
				GameObjectProjectileListAdd(clone);
			}
		}
	}
}

void BehaviorPoolMagnetizePoint(GameObjectPtr gameObect, GameObjectPtr* orbs,int size)
{
	if (gameObect && orbs)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);

		if (behavior && behavior->isMetal && behavior->isMagnitized)
		{
			float shortestDistance = 700;
			int closestOrb = -1;
			TransformPtr monsterTrans = GameObjectGetTransform(gameObect);
			const Vector2D * monsterTransl = TransformGetTranslation(monsterTrans);
			PhysicsPtr monsterPhys = GameObjectGetPhysics(gameObect);

			for (int i = 0; i < size; i++)
			{
				if (!orbs[i])
					continue;
				TransformPtr orbTran = GameObjectGetTransform(orbs[i]);
				Vector2D orbTransl = *TransformGetTranslation(orbTran);
				float distance = Vector2DDistance(&orbTransl, monsterTransl);

				if (distance < shortestDistance)
				{
					shortestDistance = distance;
					closestOrb = i;
				}

			}


			if (closestOrb >= 0)
			{
				if (Vector2DDistance(TransformGetTranslation(GameObjectGetTransform(orbs[closestOrb])), monsterTransl) < 10 &&
					Vector2DDistance(TransformGetTranslation(GameObjectGetTransform(orbs[closestOrb])), monsterTransl) > -10)
				{
					TransformSetTranslation(monsterTrans, TransformGetTranslation(GameObjectGetTransform(orbs[closestOrb])));
					return;
				}

				Vector2D monsterVelocity;
				Vector2D gracity = {0, 0};

				Vector2D OrbTrans = *TransformGetTranslation(GameObjectGetTransform(orbs[closestOrb]));

				Vector2DSub(&monsterVelocity, &OrbTrans, monsterTransl);
				Vector2DNormalize(&monsterVelocity, &monsterVelocity);
				Vector2DScale(&monsterVelocity, &monsterVelocity, 600);

				PhysicsSetVelocity(monsterPhys, &monsterVelocity);
				PhysicsSetAcceleration(monsterPhys, &gracity);
			}
		}
	}
}

void BehaviorPoolSetFrozen(GameObjectPtr gameObect, SpriteSourcePtr iceBlockSource)
{
	if (gameObect)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);

		if (behavior)
		{
			if (behavior->isOnFire) 
			{
				behavior->isOnFire = false;
				behavior->fireTimer = 0;
				return;
			}

			if (behavior->isMagnitized)
			{
				behavior->isMagnitized = false;
			}

			behavior->isFrozen = true;
			behavior->IceBlockId = addIcePlatform(TransformGetTranslation(GameObjectGetTransform(gameObect))->x,
				TransformGetTranslation(GameObjectGetTransform(gameObect))->y,
				fabsf(TransformGetScale(GameObjectGetTransform(gameObect))->x),
				fabsf(TransformGetScale(GameObjectGetTransform(gameObect))->y),
				TransformGetRotation(GameObjectGetTransform(gameObect)),
				iceBlockSource);
			

			behavior->FrozenTimer = MONSTER_FROZEN_TIME;
		}
	}
}

void BehaviorPoolSetFire(GameObjectPtr gameObect)
{
	if (gameObect)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);
		ParticleEmitterPtr ptr = GameObjectGetEmitter(gameObect);
		if (behavior)
		{
			int fireDamage = 2;
			if (behavior->isFrozen)
			{
				deleteIceplatform(behavior->IceBlockId);
				behavior->FrozenTimer = 0;
				behavior->isFrozen = false;
			}
			else
			{
				behavior->health -= fireDamage;
				behavior->isOnFire = true;
				behavior->fireTimer = MONSTER_FIRE_TIME;
				ParticleEmitterSetActiveTimer(ptr, 2.5f);
				ParticleEmitterSetActive(ptr, true);
				ParticleEmitterSetEmitRate(ptr, 0.01f);
				ParticleEmitterSetParticle(ptr, "FireEffect");
			}
		}
	}
}

void BehaviorPoolSetMagnetize(GameObjectPtr gameObect)
{
	if (gameObect)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);
		ParticleEmitterPtr ptr = GameObjectGetEmitter(gameObect);
		if (behavior)
		{
			float elecDmg = .5f;

			if (behavior->isMetal)
			{
				behavior->isMagnitized = true;
				behavior->MagTimer = 3.0f;
				ParticleEmitterSetActiveTimer(ptr, 2.5f);
				ParticleEmitterSetActive(ptr, true);
				ParticleEmitterSetEmitRate(ptr, 0.02f);
				ParticleEmitterSetParticle(ptr, "ElecEffect");
			}
			else
				behavior->health -= elecDmg;
		}
	}
}

void BehaviorPoolOnFireTimer(GameObjectPtr gameObect, float dt)
{
	if (gameObect)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);
		if (behavior && behavior->isOnFire)
		{
			behavior->fireTimer -= (10 * dt);
			behavior->health -= (10 * dt);
		}

		if (behavior->fireTimer <= 0)
		{
			behavior->isOnFire = false;
		}
	}
}

void BehaviorPoolUnfreezeTimer(GameObjectPtr gameObect, float dt)
{
	if (gameObect)
	{
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);
		ParticleEmitterPtr ptr = GameObjectGetEmitter(gameObect);
		if (behavior && behavior->isFrozen)
		{
			behavior->FrozenTimer -= (10 * dt);
			PhysicsSetAcceleration(GameObjectGetPhysics(gameObect), &gravityNone);
			PhysicsSetVelocity(GameObjectGetPhysics(gameObect), &gravityNone);
			if (behavior->FrozenTimer < 25 && !ParticleEmitterisActive(ptr))
			{
				ParticleEmitterSetActiveTimer(ptr, 1.0f);
				ParticleEmitterSetActive(ptr, true);
				ParticleEmitterSetEmitRate(ptr, 0.1f);
				ParticleEmitterSetParticle(ptr, "IceEffect");
			}
			
		}

		if (behavior->FrozenTimer <= 0)
		{
			deleteIceplatform(behavior->IceBlockId);
			PhysicsSetAcceleration(GameObjectGetPhysics(gameObect), &gravityNormal);
			behavior->isFrozen = false;
		}
	}
}

void BehaviorPoolUpdateTimer(BehaviorPtr behavior, float dt)
{
	if (behavior)
	{
		behavior->timer -= dt;
		if (behavior->timer <= 0)
			behavior->timer = 0;
	}
}

void BehaviorPoolUpdateMagTimer(BehaviorPtr behavior, float dt)
{
	if (behavior)
	{
		behavior->MagTimer -= dt;
		if (behavior->MagTimer <= 0)
			behavior->MagTimer = 0;
	}
}

void BehaviorPoolCheckAggro(GameObjectPtr gameObect, GameObjectPtr player)
{
	if (gameObect && player)
	{
		Vector2D playerTranslation = *TransformGetTranslation(GameObjectGetTransform(player));
		Vector2D gameObectTranslation = *TransformGetTranslation(GameObjectGetTransform(gameObect));
		BehaviorPtr behavior = GameObjectGetBehavior(gameObect);
		float distance = Vector2DSquareDistance(&playerTranslation, &gameObectTranslation);
		behavior->isAggro = (behavior->aggroRange > distance);
		behavior->targetPos = playerTranslation;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

