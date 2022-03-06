#pragma once

#include "ECS/ECS.h"
#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"

#include <vector>

struct CollisionPoints;

/**
 * @brief The Interaction class specifies how two entities should interact, in the event that an
 * interaction between the two is detected by the InteractionWorld.
 * 
 * An interaction takes place between two entities, an interactor and an interactee.
 */
class Interaction
{
public:
	virtual ~Interaction() = default;
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
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents,
		const CollisionPoints& points) {}

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
	InteractionWorld(ECS& ecs) : ECSListener(), ecs(ecs)
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

	void ProcessInteraction(float deltaTime, const EntityInternal& interactor,
		const EntityInternal& interactee, const CollisionPoints& points) const;

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
