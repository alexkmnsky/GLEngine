#pragma once

#include "ECS/ECS.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "Events/MotionControl.h"
#include "Events/InputControl.h"
#include "MotionComponentSystem.h"

#include <GLM/glm.hpp>
#include <vector>
#include <algorithm>

/** @brief Struct for mouse motion data relevant to the MovementControlSystem. */
struct MovementMotionControl
{
	glm::vec3 movement;
	MotionControl* motionControl;

	MovementMotionControl(const glm::vec3& movement, MotionControl* motionControl) : movement(movement),
		motionControl(motionControl) {}
};

/** @brief Struct for key and button input data relevant to the MovementControlSystem. */
struct MovementInputControl
{
	glm::vec3 movement;
	InputControl* inputControl;
	float lastInputAmount; // Used for finding input delta

	MovementInputControl(const glm::vec3& movement, InputControl* inputControl) : movement(movement),
		inputControl(inputControl), lastInputAmount(inputControl->GetAmount()) {}
};

/** @brief Component which holds all relevant movement controls for the entity. */
struct MovementControlComponent : public ECSComponent<MovementControlComponent>
{
	std::vector<MovementMotionControl> motionControls; // Mouse motion controls
	std::vector<MovementInputControl> inputControls; // Key and button controls
};

class MovementControlSystem : public BaseECSSystem
{
public:
	MovementControlSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(MovementControlComponent::ID);
		AddComponentType(MotionComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		MovementControlComponent* movementControl = (MovementControlComponent*)components[1];
		MotionComponent* motionComponent = (MotionComponent*)components[2];

		// Iterate over all key/button input controls
		for (MovementInputControl& inputControl : movementControl->inputControls)
		{
			glm::vec3 movement = inputControl.movement;
			float inputAmount = inputControl.inputControl->GetAmount();
			float inputDelta = inputAmount - inputControl.lastInputAmount;

			// Use local acceleration, which is relative to the entity
			// A rotated version of this acceleration will then be applied to the entity
			localAcceleration += movement * inputDelta;		

			inputControl.lastInputAmount = inputAmount;
		}

		// Iterate over all mouse motion controls
		for (MovementMotionControl& motionControl : movementControl->motionControls)
		{
			glm::vec3 movement = motionControl.movement;
			float inputDelta = motionControl.motionControl->GetAmount();

			transform->transform.GetRotation() += movement * inputDelta;

			// Lock X rotation from rolling over
			transform->transform.GetRotation().x = std::min(transform->transform.GetRotation().x, 90.0f);
			transform->transform.GetRotation().x = std::max(transform->transform.GetRotation().x, -90.0f);
		}

		// Get the rotation matrix based on the entity's rotation

		glm::mat4 rotationXMatrix = glm::rotate(
			glm::radians(transform->transform.GetRotation().x), glm::vec3(1.0, 0.0, 0.0));

		glm::mat4 rotationYMatrix = glm::rotate(
			glm::radians(transform->transform.GetRotation().y), glm::vec3(0.0, 1.0, 0.0));

		glm::mat4 rotationZMatrix = glm::rotate(
			glm::radians(transform->transform.GetRotation().z), glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

		// Set the entity's acceleration to a rotated version of the local acceleration
		// This makes the player move in the direction they are facing
		// Using the Y rotation matrix locks this to only the yaw of the player
		motionComponent->acceleration = glm::vec3(
			rotationYMatrix * glm::vec4(localAcceleration, 1.0f));

		// TODO This is cheap hack to get resistance; replace with a proper system later
		motionComponent->velocity *= std::max(0.0f, 1.0f - (deltaTime * 15.0f));
	}
private:
	glm::vec3 localAcceleration = glm::vec3(0.0f);
};