#pragma once

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "GameRenderContext.h"
#include "Rendering/Camera.h"
#include "Transform.h"

/**
 * @brief Component which defines the camera being used, and how to position it relative
 * to the entity which this component is attached to. Only one camera component should be used
 * at a time to avoid issues.
 */
struct CameraComponent : public ECSComponent<CameraComponent>
{
	Camera* camera = nullptr;
	Transform offset;
};

/**
 * @brief System which updates the position and rotation of the camera according to
 * the state of the camera component.
 */
class CameraSystem : public BaseECSSystem
{
public:
	CameraSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(CameraComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		CameraComponent* camera = (CameraComponent*)components[1];

		// Update position
		camera->camera->SetPosition(
			transform->transform.GetPosition() + camera->offset.GetPosition());	
		
		// Update rotation
		camera->camera->SetRotation(
			transform->transform.GetRotation() + camera->offset.GetRotation());
	}
private:
};
