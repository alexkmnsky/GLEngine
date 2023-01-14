/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once
#include "PhysicsCollision.h"
#include "Transform.h"

struct SphereCollider;
struct PlaneCollider;

struct Collider
{
	virtual ~Collider() = default;

	virtual CollisionPoints TestCollision(const Transform* transform, const Collider* collider,
		const Transform* colliderTransform) const = 0;

	virtual CollisionPoints TestCollision(const Transform* transform, const SphereCollider* sphere,
		const Transform* sphereTransform) const = 0;

	virtual CollisionPoints TestCollision(const Transform* transform, const PlaneCollider* plane,
		const Transform* planeTransform) const = 0;
};