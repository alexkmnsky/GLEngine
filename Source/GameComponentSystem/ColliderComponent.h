#pragma once

#include "ECS/ECSComponent.h"
#include "AABB.h"
#include "Physics/SphereCollider.h"
#include "Physics/PlaneCollider.h"

#include <variant>

/** @brief Component which defines the bounds of an object. Used for collision detection. */
struct ColliderComponent : public ECSComponent<ColliderComponent>
{
	// Bounding box at world origin
	AABB aabb;

	std::variant<SphereCollider, PlaneCollider> collider;
};
