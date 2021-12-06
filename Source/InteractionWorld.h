#pragma once

#include "ECS/ECS.h"
#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"

#include <vector>

/**
 * @brief The Interaction class specifies how two entities should interact, in the event that an
 * interaction between the two is detected by the InteractionWorld.
 * 
 * An interaction takes place between two entities, an interactor and an interactee.
 */
class Interaction
{
public:
	/**
	 * Called in the event that an interaction between the interactor and interactee is detected.
	 * 
	 * @param deltaTime How much time has passed since the previous update.
	 * @param interactor Handle to the interactor entity.
	 * @param interactee Handle to the interactee entity.
	 * @param interactorComponents The array of the interactor's components. Contains only the
	 *			required component types for the interactor, and not all components attached
	 *			to the interactor entity.
	 * @param interacteeComponents The array of the interactee's components. Contains only the
	 *			required component types for the interactee, and not all components attached
	 *			to the interactee entity.
	 */
	virtual void Interact(float deltaTime, EntityHandle interactor, EntityHandle interactee, 
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents) {}

	/** @brief Called after all interactions have been processed. */
	virtual void Finalize(float deltaTime) {}

	/** @brief Gets the list of required component types for the interactor. */
	const std::vector<unsigned int>& GetInteractorComponents() { return interactorComponentTypes; }

	/** @brief Gets the list of required component types for the interactee. */
	const std::vector<unsigned int>& GetInteracteeComponents() { return interacteeComponentTypes; }

protected:
	/**
	 * Adds a component type to the list of required component types for the interactor.
	 * 
	 * @param type The ID of the component type.
	 */
	void AddInteractorComponentType(unsigned int type)
	{
		interactorComponentTypes.push_back(type);
	}

	/**
	 * Adds a component type to the list of required component types for the interactee.
	 *
	 * @param type The ID of the component type.
	 */
	void AddInteracteeComponentType(unsigned int type)
	{
		interacteeComponentTypes.push_back(type);
	}

private:
	std::vector<unsigned int> interactorComponentTypes;
	std::vector<unsigned int> interacteeComponentTypes;
};

class InteractionWorld : public ECSListener
{
public:
	InteractionWorld(ECS& ecs) : ECSListener(), ecs(ecs), compareAABB(ecs, 0)
	{
		// Listen to all component operations
		SetNotificationSettings(true, false);
		// For collision detection, we need a TransformComponent and a ColliderComponent
		AddComponentID(TransformComponent::ID);
		AddComponentID(ColliderComponent::ID);
	}

	/** @see ECSListener::OnMakeEntity */
	virtual void OnMakeEntity(EntityHandle handle);

	/** @see ECSListener::OnRemoveEntity */
	virtual void OnRemoveEntity(EntityHandle handle);

	/** @see ECSListener::OnAddComponent */
	virtual void OnAddComponent(EntityHandle handle, unsigned int id);

	/** @see ECSListener::OnRemoveComponent */
	virtual void OnRemoveComponent(EntityHandle handle, unsigned int id);

	/**
	 * Proccesses interactions between entities. Should be called every update.
	 * 
	 * @param deltaTime How much time has passed since the previous update.
	 */
	void ProcessInteractions(float deltaTime);

	/**
	 * Adds an interaction to the InteractionWorld. The interaction determines how two entities
	 * should interact, in the event that an interaction between the two is detected.
	 * 
	 * @param interaction Pointer to the interaction to add.
	 */
	void AddInteraction(Interaction* interaction);

private:
	/** @brief Used internally for referring to an entity. */
	struct EntityInternal
	{
		EntityHandle handle;

		// The indices of the interactions in which this entity is the interactor
		std::vector<unsigned int> interactors;

		// The indices of the interactions in which this entity is the interactee
		std::vector<unsigned int> interactees;
	};

	/**
	 * @brief Functor which is used to determine, along the specified axis, if the min extents of 
	 * one entity's AABB is smaller than the min extents of another entity's AABB.
	 * 
	 * This is useful for sorting the bounding boxes of entities along the max variance axis, which
	 * allows for the implementation of the sort and sweep algorithm.
	 */
	struct InteractionWorldCompare
	{
		// If axis is set to 0, the X axis will be used
		// If axis is set to 1, the Y axis will be used
		// If axis is set to 2, the Z axis will be used
		unsigned int axis = 0;
		ECS& ecs;

		/**
		 * @param ecs Reference to the ECS, which is used to retrieve the components from entities.
		 * @param axis The axis to perform comparisons on.
		 */
		InteractionWorldCompare(ECS& ecs, unsigned int axis) : ecs(ecs), axis(axis) {}

		/** @brief Returns true if entity a's min extent is less than entity b's min extent. */
		bool operator()(EntityInternal& a, EntityInternal& b)
		{
			float aMin = ecs.GetComponent<ColliderComponent>(
				a.handle)->transformedAABB.GetMinExtents()[axis];
			float bMin = ecs.GetComponent<ColliderComponent>(
				b.handle)->transformedAABB.GetMinExtents()[axis];

			return (aMin < bMin);
		}
	};

	std::vector<EntityInternal> entities;

	// List of entities to remove
	// Every time interactions are processed, the entities in the list will be removed, and the
	// list will be cleared
	std::vector<EntityHandle> entitiesToRemove;

	// List of entities to update
	// Every time interactions are processed, the entities in the list will be updated, and the
	// list will be cleared
	std::vector<EntityHandle> entitiesToUpdate;

	std::vector<Interaction*> interactions;

	ECS& ecs;
	InteractionWorldCompare compareAABB;

	void RemoveAndUpdateEntities();

	/**
	 * Adds an entity to the InteractionWorld. Ensures that interactions are computed for the
	 * entity.
	 * 
	 * @param handle Handle to the entity being added.
	 */
	void AddEntity(EntityHandle handle);

	/**
	 * Computes if the entity is an interactor, and if the entity is an interactee, for the
	 * interaction at the specified index. If the entity is an interactor and/or an interactee, 
	 * the interaction index will be pushed back accordingly.
	 * 
	 * This is computed only when needed for efficient lookup when processing interactions.
	 * 
	 * @see EntityInternal::interactors
	 * @see EntityInternal::interactees
	 * 
	 * @param entity Entity to compute interactions for.
	 * @param interactionIndex Index in the interactions vector/array.
	 */
	void ComputeInteractions(EntityInternal& entity, unsigned int interactionIndex);
};
