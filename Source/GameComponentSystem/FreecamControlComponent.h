#pragma once

#include "ECS/ECS.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "Events/AxisControl.h"
#include "Events/MotionControl.h"
#include "MotionComponentSystem.h"

#include <GLM/glm.hpp>

struct FreecamControlComponent : public ECSComponent<FreecamControlComponent>
{
	AxisControl* forward;
	AxisControl* right;
	MotionControl* motion;
};

class FreecamControlSystem : public BaseECSSystem
{
public:
	FreecamControlSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(FreecamControlComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		const auto transformComponent = (TransformComponent*)components[0];
		const auto freecamControlComponent = (FreecamControlComponent*)components[1];
		const auto motionComponent = (MotionComponent*)components[2];

		glm::vec3& position = transformComponent->transform.GetPosition();
		glm::vec3& rotation = transformComponent->transform.GetRotation();

		rotation.y -= freecamControlComponent->motion->Get().x;
		rotation.x -= freecamControlComponent->motion->Get().y;

		glm::vec3 forward(0.f, 0.f, -1.f);
		forward = glm::rotate(forward, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		forward = glm::rotate(forward, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward = glm::rotate(forward, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 right(1.f, 0.f, 0.f);
		right = glm::rotate(right, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		right = glm::rotate(right, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		right = glm::rotate(right, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		position += forward * freecamControlComponent->forward->Get() * deltaTime;
		position += right * freecamControlComponent->right->Get() * deltaTime;
	}
};