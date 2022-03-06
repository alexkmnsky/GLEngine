#pragma once

#include "ECS/ECS.h"
#include "MotionIntegrators.h"

#include <GLM/glm.hpp>

/** @brief Component which defines how an entity should move in the world. */
struct MotionComponent : public ECSComponent<MotionComponent>
{
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
};

/**
 * @brief System which updates the position of the entity according to the state of the motion
 * component. Uses a motion integrator to determine the new position every update.
 */
class MotionSystem : public BaseECSSystem
{
public:
	MotionSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(MotionComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		MotionComponent* motion = (MotionComponent*)components[1];

		// Update position of the entity...
		glm::vec3 newPosition = transform->transform.GetPosition();
		MotionIntegrators::ModifiedEuler(newPosition, motion->velocity,
			motion->force, deltaTime);
		transform->transform.SetPosition(newPosition);
	}

private:
};