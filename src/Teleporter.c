//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "Physics.h"
#include "Teleporter.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void TeleporterUpdateObject(GameObjectPtr gameObject)
{
	if (gameObject)
	{
		float MaxX = AEGfxGetWinMaxX() - 40;
		float MaxY = AEGfxGetWinMaxY() - 40;
		PhysicsPtr physic = GameObjectGetPhysics(gameObject);
		TransformPtr transform = GameObjectGetTransform(gameObject);
		if (physic && transform)
		{
			Vector2D position = *TransformGetTranslation(transform);
			const Vector2D * size = TransformGetScale(transform);
			const Vector2D * velocity = PhysicsGetVelocity(physic);

			if (velocity->x > 0)
			{
				if (position.x - size->x / 2 > MaxX)
					position.x = -MaxX - (position.x - MaxX);
			}
			else if (velocity->x < 0)
			{
				if (position.x + size->x / 2 < -MaxX)
					position.x = MaxX + (-MaxX - position.x);
			}

			if (velocity->y > 0)
			{
				if (position.y - size->y / 2 > MaxY)
					position.y = -MaxY - (position.y - MaxY);
			}
			else if (velocity->y < 0)
			{
				if (position.y + size->y / 2 < -MaxY)
					position.y = MaxY + (-MaxY - position.y);
			}

			TransformSetTranslation(transform, &position);
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------