/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

#include "InteractionWorld.h"

#include "Algorithm/Octree.h"
#include "Physics/PhysicsCollision.h"

#include <algorithm>

void InteractionWorld::OnMakeEntity(EntityHandle handle)
{
	// OnMakeEntity is only called if the entity contains both a transform and collider component

	// Compute interactions for the newly added entity, and add the entity to the list of entities
	AddEntity(handle);
}

void InteractionWorld::OnRemoveEntity(EntityHandle handle)
{
	// OnRemoveEntity is only called if the entity contains both a transform and collider component
	// This means that this entity should be removed

	// Add the entity to the list of entities to remove
	entitiesToRemove.push_back(handle);
}

void InteractionWorld::OnAddComponent(EntityHandle handle, unsigned int id)
{
	// If a transform component was added to the entity
	if (id == TransformComponent::ID)
	{
		// If the entity already has a collider component
		if (ecs.GetComponent<ColliderComponent>(handle) != nullptr)
		{
			AddEntity(handle);
		}
	}
	// If a collider component was added to the entity
	else if (id == ColliderComponent::ID)
	{
		// If the entity already has a transform component
		if (ecs.GetComponent<TransformComponent>(handle) != nullptr)
		{
			AddEntity(handle);
		}
	}
	// If another component was added to the entity, and the entity already has both a collider
	// component and a transform component
	else if (ecs.GetComponent<ColliderComponent>(handle) != nullptr &&
		ecs.GetComponent<TransformComponent>(handle) != nullptr)
	{
		// The entity needs to be updated so that interactions are computed
		// See RemoveAndUpdateEntities
		entitiesToUpdate.push_back(handle);
	}
}

void InteractionWorld::OnRemoveComponent(EntityHandle handle, unsigned int id)
{
	// If either a transform component or a transform component is being removed, then the
	// InteractionWorld is not longer interested in this entity and it can be removed
	if (id == TransformComponent::ID || id == ColliderComponent::ID)
	{
		// Add the entity to the list of entities to remove
		entitiesToRemove.push_back(handle);
	}
	// If another component is being removed, and the entity has both a collider component and a
	// transform component
	else if (ecs.GetComponent<ColliderComponent>(handle) != nullptr &&
		ecs.GetComponent<TransformComponent>(handle) != nullptr)
	{
		// The entity needs to be updated so that interactions are computed
		// See RemoveAndUpdateEntities
		entitiesToUpdate.push_back(handle);
	}
}

void InteractionWorld::ProcessInteractions(float deltaTime)
{
	std::vector<BaseECSComponent*> interactorComponents;
	std::vector<BaseECSComponent*> interacteeComponents;

	// Remove entitiesToRemove and update entitiesToUpdate
	RemoveAndUpdateEntities();

	std::vector<std::pair<EntityHandle, const AABB>> data;
	float min =  std::numeric_limits<float>::infinity();
	float max = -std::numeric_limits<float>::infinity();

	for (const EntityInternal& entity : entities)
	{
		// Get the collider component of the entity
		const auto colliderComponent = ecs.GetComponent<ColliderComponent>(entity.handle);

		const AABB transformedAABB = colliderComponent->aabb.Translate(
			ecs.GetComponent<TransformComponent>(entity.handle)->transform.GetPosition());

		data.emplace_back(entity.handle, transformedAABB);

		const glm::vec3 minExtents = transformedAABB.GetMinExtents();
		const glm::vec3 maxExtents = transformedAABB.GetMaxExtents();
		min = std::min({ min, minExtents.x, minExtents.y, minExtents.z });
		max = std::max({ max, maxExtents.x, maxExtents.y, maxExtents.z });
	}

	auto octree = Algorithm::Octree<EntityHandle>(AABB(glm::vec3(min), glm::vec3(max)));

	for (const auto& [handle, objectBounds] : data)
	{
		octree.Insert(handle, objectBounds);
	}

	std::vector<Collision<EntityInternal&>> collisions;

	// Go through the list of entities, test intersections in range
	for (size_t i = 0; i < entities.size(); i++)
	{
		// Find intersections for this entity...

		// Iterate over entities, start at i + 1
		// As the order of the two does not matter, this will ensure that i and j will be a
		// unique pair of entities

		for (size_t j = i + 1; j < entities.size(); j++)
		{

			if (!octree.AreRelated(entities[i].handle, entities[j].handle))
			{
				continue; 
			}

			const auto transformComponentA = ecs.GetComponent<TransformComponent>(entities[i].handle);
			const auto transformComponentB = ecs.GetComponent<TransformComponent>(entities[j].handle);

			const auto colliderComponentA = ecs.GetComponent<ColliderComponent>(entities[i].handle);
			const auto colliderComponentB = ecs.GetComponent<ColliderComponent>(entities[j].handle);

			Collider* colliderA;
			Collider* colliderB;

			// https://stackoverflow.com/a/53166942
			std::visit([&colliderA](auto&& collider) { colliderA = &collider; }, colliderComponentA->collider);
			std::visit([&colliderB](auto&& collider) { colliderB = &collider; }, colliderComponentB->collider);

			CollisionPoints points = colliderA->TestCollision(&transformComponentA->transform,
				colliderB, &transformComponentB->transform);

			if (points.isColliding)
			{
				collisions.emplace_back(entities[i], entities[j], points);
			}
		}
	}

	for (const auto& [interactor, interactee, points] : collisions)
	{
		ProcessInteraction(deltaTime, interactor, interactee, points);
		ProcessInteraction(deltaTime, interactee, interactor, points);
	}
}

void InteractionWorld::ProcessInteraction(float deltaTime, const EntityInternal& interactor,
	const EntityInternal& interactee, const CollisionPoints& points) const
{
	std::vector<BaseECSComponent*> interactorComponents;
	std::vector<BaseECSComponent*> interacteeComponents;

	// Iterate over all interactions where the interactor has all the required component types
	for (const unsigned int interactorInteraction : interactor.interactors)
	{
		// Iterate over all interactions where the interactee has all the required component types
		for (const unsigned int interacteeInteraction : interactee.interactees)
		{
			// If both indices match, an interaction where the interactor has all required component
			// types and the interactee has all required component types has been found
			if (interactorInteraction == interacteeInteraction)
			{
				// Get the interaction, which determines how the two entities should interact
				Interaction* interaction = interactions[interactorInteraction];

				// Resize the component vectors to fit the entities' components
				interactorComponents.resize(std::max(interactorComponents.size(),
					interaction->GetInteractorComponents().size()));
				interacteeComponents.resize(std::max(interacteeComponents.size(),
					interaction->GetInteracteeComponents().size()));

				// Iterate over all required component types for the interactor
				for (size_t i = 0; i < interaction->GetInteractorComponents().size(); i++)
				{
					// Get the component of the type from the interactor entity
					// Add it to the interactor components list
					interactorComponents[i] = ecs.GetComponentByType(interactor.handle,
						interaction->GetInteractorComponents()[i]);
				}

				// Iterate over all required component types for the interactee
				for (size_t i = 0; i < interaction->GetInteracteeComponents().size(); i++)
				{
					// Get the component of the type from the interactee entity
					// Add it to the interactee components list
					interacteeComponents[i] = ecs.GetComponentByType(interactee.handle,
						interaction->GetInteracteeComponents()[i]);
				}

				// Pass in all relevant data to the interaction
				interaction->Interact(deltaTime, interactor.handle, interactee.handle,
					interactorComponents.data(), interacteeComponents.data(), points);
			}
		}
	}
}

void InteractionWorld::AddInteraction(Interaction* interaction)
{
	// Add the interaction
	interactions.push_back(interaction);

	// Get the index of the newly added interaction
	size_t index = interactions.size() - 1;

	// Iterate over all entities in the InteractionWorld
	for (size_t i = 0; i < entities.size(); i++)
	{
		// Compute interactions with the newly added interaction
		ComputeInteractions(entities[i], index);
	}
}

void InteractionWorld::RemoveAndUpdateEntities()
{
	// If there are no entities to remove or update, return
	if (entitiesToRemove.size() == 0 && entitiesToUpdate.size() == 0) return;

	// Iterate over all entities in the InteractionWorld
	// Update entities after removing; there is no use in updating an entity if it is being removed
	for (size_t i = 0; i < entities.size(); i++)
	{
		// Remove entities...

		bool didRemove = false;
		// If the two entities are swapped (for removal), the check needs to performed again
		// Continue checking until the entities are not swapped (not removed)
		do {
			didRemove = false;
			// Iterate over all entities to remove
			for (size_t j = 0; j < entitiesToRemove.size(); j++)
			{
				// If the two entities match, remove the entity
				if (entities[i].handle == entitiesToRemove[j])
				{
					// Swap the entity being removed with the last element in the entities list
					std::swap(entities[i], entities[entities.size() - 1]);
					// Remove the last element
					entities.pop_back();

					// Swap the entity being removed with the last element in the
					// entitiesToRemove list
					std::swap(entitiesToRemove[j], entitiesToRemove[entitiesToRemove.size() - 1]);
					// Remove the last element
					entitiesToRemove.pop_back();

					// The entities were swapped; this check needs to be performed again
					didRemove = true;
					break;
				}
			}
			// If there are no more entities to remove or update, return
			if (entitiesToRemove.size() == 0 && entitiesToUpdate.size() == 0)
			{
				return;
			}
		} while (didRemove);

		// Update entities...

		// Iterate over all entities to update
		for (size_t j = 0; j < entitiesToUpdate.size(); j++)
		{
			// If the two entities match, update the entity
			if (entities[i].handle == entitiesToUpdate[j])
			{
				// Reset interactors and interactees; they will be recomputed
				entities[i].interactors.clear();
				entities[i].interactees.clear();

				// Iterate over all interactions
				for (size_t k = 0; k < interactions.size(); k++)
				{
					// Compute interactions for the entity with each interaction
					ComputeInteractions(entities[i], k);
				}

				// Swap the entity being updated with the last element in the entitiesToUpdate list
				std::swap(entitiesToUpdate[j], entitiesToUpdate[entitiesToUpdate.size() - 1]);
				// Remove the last element
				entitiesToUpdate.pop_back();
			}
		}
	}
	// Clear the lists...
	// Theoretically these should be empty after this process, but just in case, cleanup for the
	// next update because the entities do not exist
	entitiesToRemove.clear();
	entitiesToUpdate.clear();
}

void InteractionWorld::AddEntity(EntityHandle handle)
{
	// Create an EntityInternal which can hold interaction data
	// See declaration of the struct for more info
	EntityInternal entity;
	// Set the handle in the internal format
	entity.handle = handle;
	// Loop over all interaction indices
	for (size_t i = 0; i < interactions.size(); i++)
	{
		// Compute the interactions for the entity being added, at the particular interaction index
		ComputeInteractions(entity, i);
	}
	// Add the entity to the entities list
	entities.push_back(entity);
}

void InteractionWorld::ComputeInteractions(EntityInternal& entity, unsigned int interactionIndex)
{
	// Get a pointer to the interaction at the specified index
	Interaction* interaction = interactions[interactionIndex];

	// Check if the entity is an interactor for this interaction...
	bool isInteractor = true;
	// Iterate over all required component types for the interactor
	for (size_t i = 0; i < interaction->GetInteractorComponents().size(); i++)
	{
		// If the entity does not contain the required component
		if (ecs.GetComponentByType(entity.handle,
			interaction->GetInteractorComponents()[i]) == nullptr)
		{
			// It cannot be an interactor
			isInteractor = false;
			break;
		}
	}

	// Check if the entity is an interactee for this interaction...
	bool isInteractee = true;
	// Iterate over all required component types for the interactee
	for (size_t i = 0; i < interaction->GetInteracteeComponents().size(); i++)
	{
		// If the entity does not contain the required component
		if (ecs.GetComponentByType(entity.handle,
			interaction->GetInteracteeComponents()[i]) == nullptr)
		{
			// It cannot be an interactee
			isInteractee = false;
			break;
		}
	}

	// If the entity is an interactor, add the index of this interaction to the list of interactors
	if (isInteractor)
	{
		entity.interactors.push_back(interactionIndex);
	}
	// If the entity is an interactee, add the index of this interaction to the list of interactees
	if (isInteractee)
	{
		entity.interactees.push_back(interactionIndex);
	}
}

