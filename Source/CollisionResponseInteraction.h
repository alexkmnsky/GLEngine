#pragma once

#include "InteractionWorld.h"
#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"
#include "GameComponentSystem/MotionComponentSystem.h"

#include <GLM/glm.hpp>
#include <vector>
#include <map>

/**
 * @brief Interaction which determines how two entities, one being static (without a motion
 * component), should resolve collision.
 */
class CollisionResponseInteraction : public Interaction
{
private:
	/** @brief Internal struct used for storing the collision resolution state for each entity. */
	struct CollisionResponseData
	{
		glm::vec3& position;
		glm::vec3& velocity;
		glm::vec3 normal;
		float minEntryTime;
	};

public:
	CollisionResponseInteraction() : Interaction()
	{
		AddInteractorComponentType(TransformComponent::ID);
		AddInteractorComponentType(ColliderComponent::ID);
		AddInteractorComponentType(MotionComponent::ID);
		AddInteracteeComponentType(TransformComponent::ID);
		AddInteracteeComponentType(ColliderComponent::ID);
	}

	/** @see Interaction::Interact */
	virtual void Interact(float deltaTime, EntityHandle interactor, EntityHandle interactee,
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents);

	/** @see Interaction::Finalize */
	virtual void Finalize(float deltaTime);

private:
	// Stores the state of collision response for each entity, so that when Finalize is called, we
	// can use the optimal collision response
	// This is used to circumvent the issue of multiple collisions resulting in a poor collision 
	// response; we find the optimal collision response to use.
	std::map<EntityHandle, CollisionResponseData*> collisionResponses;
	std::vector<CollisionResponseData> responseData;
};

