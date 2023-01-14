/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include "ECS/ECS.h"
#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/MotionComponentSystem.h"
#include "Physics/Components/RigidbodyComponent.h"

#include <GLM/glm.hpp>
#include <iostream> // TODO: remove

class PhysicsWorldSystem : public BaseECSSystem
{
public:
	PhysicsWorldSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(RigidbodyComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		const auto transform = (TransformComponent*)components[0];
		const auto rigidbody = (RigidbodyComponent*)components[1];

		rigidbody->force += rigidbody->mass * gravity;

		rigidbody->velocity += rigidbody->force / rigidbody->mass * deltaTime;
		transform->transform.GetPosition() += rigidbody->velocity * deltaTime;

		rigidbody->force = glm::vec3(0, 0, 0);
	}

private:
	glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
};
