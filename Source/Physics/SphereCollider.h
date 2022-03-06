#pragma once

#include "Collider.h"
#include "PhysicsCollision.h"

#include <GLM/glm.hpp>

struct SphereCollider : public Collider
{
	CollisionPoints TestCollision(const Transform* transform, const Collider* collider,
		const Transform* colliderTransform) const override
	{
		return collider->TestCollision(colliderTransform, this, transform);
	}

	CollisionPoints TestCollision(const Transform* transform,  const SphereCollider* sphere,
		const Transform* sphereTransform) const override
	{
		return CollisionTest::FindSphereSphereCollisionPoints(this, transform, sphere, sphereTransform);
	}

	CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* plane,
		const Transform* planeTransform) const override
	{
		return CollisionTest::FindSpherePlaneCollisionPoints(this, transform, plane, planeTransform);
	}

	glm::vec3 center;
	float radius;
};