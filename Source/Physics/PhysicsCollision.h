/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include <GLM/glm.hpp>

struct SphereCollider;
struct PlaneCollider;
class Transform;

struct CollisionPoints
{
	glm::vec3 a; // Furthest point of A into B
	glm::vec3 b; // Furthest point of B into A
	glm::vec3 normal; // B - A normalized
	float depth; // Length of B - A
	bool isColliding;
};
 
namespace CollisionTest
{
	CollisionPoints FindSphereSphereCollisionPoints(const SphereCollider* a, 
		const Transform* transformA, const SphereCollider* b, const Transform* transformB);

	CollisionPoints FindSpherePlaneCollisionPoints(const SphereCollider* a, 
		const Transform* transformA, const PlaneCollider* b, const Transform* transformB);
}

template<typename Handle>
struct Collision
{
	Handle objectA;
	Handle objectB;
	CollisionPoints points;

	Collision(Handle objectA, Handle objectB, CollisionPoints points) : objectA(objectA),
		objectB(objectB), points(points) {}
};
