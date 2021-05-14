//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	QIAOCHU LIU, Garry Chen
// Project:		ArcaneEscape
// Course:		GAM150S19-c
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"
#include "Behavior.h"

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

// Dynamically allocate a clone of an existing behavior.
// (Hint: Perform a shallow copy of the member variables, then change the 'parent' pointer.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
BehaviorPtr BehaviorClone(BehaviorPtr other, GameObjectPtr parent) 
{
	BehaviorPtr behavior = calloc(1,sizeof(Behavior));

	if (behavior && other)
	{
		*behavior = *other;
		behavior->parent = parent;
		if(behavior->cloneData)
			behavior->cloneData(behavior);
	}

	return behavior;
}

// Free the memory associated with a behavior component.
// (Also, set the behavior pointer to NULL.)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorFree(BehaviorPtr * behavior) 
{
	free(*behavior);
	*behavior = NULL;
}

// Update the behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUpdate(BehaviorPtr behavior, float dt) 
{
	if (behavior)
	{
		if (behavior->stateCurr != behavior->stateNext) 
		{
			if (behavior->onExit)
			{
				behavior->onExit(behavior);
			}
			behavior->stateCurr = behavior->stateNext;
			if (behavior->onInit)
			{
				behavior->onInit(behavior);
			}
		}
		if (behavior->onUpdate)
		{
			behavior->onUpdate(behavior, dt);
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

