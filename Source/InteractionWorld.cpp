#include "InteractionWorld.h"
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

	// Values used to find the highest variance axis
	glm::vec3 centerSum;
	glm::vec3 centerSquareSum;

	// Perform this check three times; for each axis
	// This is needed for the collision response, as a resolution on one axis can result in a 
	// collision on another
	// TODO Find a better solution for this... (this is essentially a hack)
	for (unsigned int axis = 0; axis < 3; axis++)
	{
		// Remove entitiesToRemove and update entitiesToUpdate
		RemoveAndUpdateEntities();

		// Iterate over all entities and update the transformed bounding box
		for (size_t i = 0; i < entities.size(); i++)
		{
			// Get the collider component of the entity
			ColliderComponent* colliderComponent = ecs.GetComponent<ColliderComponent>(
				entities[i].handle);

			// Translate the collider AABB by the entity's translation and set it as the 
			// transformed AABB
			colliderComponent->transformedAABB = colliderComponent->aabb.Translate(
				ecs.GetComponent<TransformComponent>(entities[i].handle)->transform.GetPosition());
		}

		// Sort entities by min on highest variance axis
		std::sort(entities.begin(), entities.end(), compareAABB);

		// Reset the values used for find the highest variance axis every update
		centerSum = glm::vec3(0.0f);
		centerSquareSum = glm::vec3(0.0f);

		// Go through the list of entities, test intersections in range
		for (size_t i = 0; i < entities.size(); i++)
		{
			// Get the bounding box of the entity
			AABB aabb = ecs.GetComponent<ColliderComponent>(entities[i].handle)->transformedAABB;

			// Get the center of the entity's bounding box, and add the value to centerSum, and
			// the squared value to centerSquareSum (used for finding the highest variance axis)
			glm::vec3 center = aabb.GetCenter();
			centerSum += center;
			centerSquareSum += (center * center);

			// Find intersections for this entity...

			// Iterate over entities, start at i + 1
			// As the order of the two does not matter, this will ensure that i and j will be a
			// unique pair of entities
			for (size_t j = i + 1; j < entities.size(); j++)
			{
				// Get the bounding box of the other entity
				AABB otherAABB = ecs.GetComponent<ColliderComponent>(
					entities[j].handle)->transformedAABB;

				// Check for overlap on the max variance axis
				// If this is true, the bounding boxes are not colliding, and since the entities
				// are sorted along the max variance axis, no other entities beyond this one
				// can be intersecting with this entity
				if (otherAABB.GetMinExtents()[compareAABB.axis] > aabb.GetMaxExtents()[compareAABB.axis])
				{
					// Stop checking for collisions with entity i; continue to the next entity
					break;
				}

				// At this point, aabb is potentially intersecting otherAABB...
				// Perform the test
				if (aabb.Intersects(otherAABB))
				{
					size_t interactorIndex = i;
					size_t interacteeIndex = j;

					// If rules say so, then entities[i] interacts with entities[j]
					// If rules say so, then entities[j] interacts with entities[i]
					// Check if there is an interaction between the two entities...

					// Repeat twice...
					// Once where entities[i] is the interactor and entities[j] is the interactee
					// Once where entities[j] is the interactor and entities[i] is the interactee
					for (size_t dummyIndex = 0; dummyIndex < 2; dummyIndex++)
					{
						// Iterate over all interactions where the interactor has all the
						// required component types
						for (size_t k = 0; k < entities[interactorIndex].interactors.size(); k++)
						{
							// Iterate over all interactions where the interactee has all the
							// required component types
							for (size_t l = 0; l < entities[interacteeIndex].interactees.size(); l++)
							{
								unsigned int index = entities[interactorIndex].interactors[k];

								// If both indices match, an interaction where the interactor has 
								// all required component types and the interactee has all required
								// component types has been found
								if (index == entities[interacteeIndex].interactees[l])
								{
									// Get the interaction, which determines how the two entities
									// should interact
									Interaction* interaction = interactions[index];

									// Resize the component arrays to fit the entities' components
									interactorComponents.resize(std::max(interactorComponents.size(),
										interaction->GetInteractorComponents().size()));
									interacteeComponents.resize(std::max(interacteeComponents.size(),
										interaction->GetInteracteeComponents().size()));

									// Iterate over all required component types for the interactor
									for (size_t m = 0; m < interaction->GetInteractorComponents().size(); m++)
									{
										// Get the component of the type from the interactor entity
										// Add it to the interactor components list
										interactorComponents[m] = ecs.GetComponentByType(
											entities[interactorIndex].handle,
											interaction->GetInteractorComponents()[m]);
									}

									// Iterate over all required component types for the interactee
									for (size_t m = 0; m < interaction->GetInteracteeComponents().size(); m++)
									{
										// Get the component of the type from the interactee entity
										// Add it to the interactee components list
										interacteeComponents[m] = ecs.GetComponentByType(
											entities[interacteeIndex].handle,
											interaction->GetInteracteeComponents()[m]);
									}

									// Pass in all relelvant data to the interaction
									interaction->Interact(deltaTime, entities[interactorIndex].handle,
										entities[interacteeIndex].handle, interactorComponents.data(),
										interacteeComponents.data());
								}
							}
						}
						// Check the other possiblity: 
						// If the first entity is the interactee instead of the interactor
						// Swap the indices of the two
						size_t tempIndex = interactorIndex;
						interactorIndex = interacteeIndex;
						interacteeIndex = tempIndex;
					}
				}
			}
		}
		// Interactions have been processed, finalize
		for (Interaction* interaction : interactions)
		{
			interaction->Finalize(deltaTime);
		}
	}

	// Find and set max variance axis...
	// TODO This lags behind by one frame

	// Find the variance for each axis
	centerSum /= (float)entities.size();
	centerSquareSum /= (float)entities.size();
	glm::vec3 variance = centerSquareSum - (centerSum * centerSum);

	// From each of three axis, find the axis with the greatest variance
	float maxVariance = variance[0];
	unsigned int maxVarianceAxis = 0;
	if (variance[1] > maxVariance)
	{
		maxVariance = variance[1];
		maxVarianceAxis = 1;
	}
	if (variance[2] > maxVariance)
	{
		maxVariance = variance[2];
		maxVarianceAxis = 2;
	}

	// Set the axis to the one with the greatest variance
	compareAABB.axis = maxVarianceAxis;
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

				// Swap the entity being updated with the last element in the entitiesToUpdate
				// list
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

