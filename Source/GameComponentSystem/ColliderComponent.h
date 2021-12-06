#pragma once

#include "ECS/ECSComponent.h"
#include "AABB.h"

/** @brief Component which defines the bounds of an object. Used for collision detection. */
struct ColliderComponent : public ECSComponent<ColliderComponent>
{
	// Bounding box at world origin
	AABB aabb;
	// Bounding box with the object's transformation applied
	AABB transformedAABB;
	// If true, collisions should be reported but not resolved
	bool shouldOverlap = false;
};
