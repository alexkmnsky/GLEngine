/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#include "PhysicsCollision.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "Transform.h"

#include <GLM/gtx/component_wise.hpp>

CollisionPoints CollisionTest::FindSphereSphereCollisionPoints(const SphereCollider* a, 
	const Transform* transformA, const SphereCollider* b, const Transform* transformB)
{	
	CollisionPoints result;

	const glm::vec3 aPosition = a->center + transformA->GetPosition();
	const glm::vec3 bPosition = b->center + transformB->GetPosition();

	const float aRadius = a->radius * glm::compMax(transformA->GetScale());
	const float bRadius = b->radius * glm::compMax(transformB->GetScale());

	const glm::vec3 aToB = bPosition - aPosition;
	const glm::vec3 bToA = aPosition - bPosition;
	
	if (glm::length(aToB) > aRadius + bRadius)
	{
		result.isColliding = false;
		return result;
	}

	result.normal = glm::normalize(aToB);
	result.a = aPosition + result.normal * aRadius;
	result.b = bPosition - result.normal * bRadius;
	result.depth = glm::length(result.a - result.b);
	result.isColliding = true;

	return result;
}

CollisionPoints CollisionTest::FindSpherePlaneCollisionPoints(const SphereCollider* a, 
	const Transform* transformA, const PlaneCollider* b, const Transform* transformB)
{
	CollisionPoints result;
	result.isColliding = false;
	return result;
}