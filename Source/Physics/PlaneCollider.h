#pragma once

#include "Collider.h"
#include "PhysicsCollision.h"

#include <GLM/glm.hpp>

struct PlaneCollider : public Collider
{
	CollisionPoints TestCollision(const Transform* transform, const Collider* collider, 
		const Transform* colliderTransform) const override
	{
		return CollisionPoints();
	}

	CollisionPoints TestCollision(const Transform* transform, const SphereCollider* sphere, 
		const Transform* sphereTransform) const override
	{
		return CollisionPoints();
	}

	CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* plane, 
		const Transform* planeTransform) const override
	{
		return CollisionPoints();
	}

	glm::vec3 plane;
	float distance;
};