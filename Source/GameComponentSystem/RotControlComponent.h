#pragma once

#include "ECS/ECS.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "Events/AxisControl.h"
#include "Events/MotionControl.h"
#include "MotionComponentSystem.h"

#include <GLM/glm.hpp>
#include <vector>
#include <algorithm>

struct RotControlComponent : public ECSComponent<RotControlComponent>
{
	AxisControl* x;
	AxisControl* y;
	MotionControl* motion;
};

class RotControlSystem : public BaseECSSystem
{
public:
	RotControlSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(RotControlComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		RotControlComponent* rotControl = (RotControlComponent*)components[1];
		MotionComponent* motionComponent = (MotionComponent*)components[2];

		const glm::ivec2 motion = rotControl->motion->Get();
		const float deltaX = motion.x + (rotControl->x->Get() * deltaTime);
		const float deltaY = motion.y + (rotControl->y->Get() * deltaTime);

		transform->transform.GetRotation().y += deltaX;
		transform->transform.GetRotation().x += deltaY;
	}
};