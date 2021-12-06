#pragma once

#include "ECS/ECSComponent.h"
#include "Transform.h"

#include <string>

/** @brief Component which defines the position, rotation, and scale of an object. */
struct TransformComponent : public ECSComponent<TransformComponent>
{
	Transform transform;
};