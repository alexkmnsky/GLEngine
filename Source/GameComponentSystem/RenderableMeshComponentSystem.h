#pragma once

#include "ECS/ECS.h"
#include "TransformComponent.h"
#include "Rendering/GameRenderContext.h"

/** @brief Component which defines the visible mesh of an entity. */
struct RenderableMeshComponent : public ECSComponent<RenderableMeshComponent>
{
	// The mesh to use
	Mesh* mesh = nullptr;

	// The texture to apply onto the mesh
	Texture* texture = nullptr;
};

/** @brief System which draws visible mesh of the entity every update. */
class RenderableMeshSystem : public BaseECSSystem
{
public:
	/**
	 * @param context The game render context, which bridges the gap between the high-level and
	 * low-level rendering commands
	 */
	RenderableMeshSystem(GameRenderContext& context) : BaseECSSystem(), context(context)
	{
		AddComponentType(TransformComponent::ID);
		AddComponentType(RenderableMeshComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];
		RenderableMeshComponent* mesh = (RenderableMeshComponent*)components[1];

		context.RenderMesh(*mesh->mesh, *mesh->texture, transform->transform);
	}
private:
	GameRenderContext& context;
};
