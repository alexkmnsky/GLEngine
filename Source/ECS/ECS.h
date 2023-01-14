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

#pragma once

#include "ECSComponent.h"
#include "ECSSystem.h"

#include <unordered_map>
#include <vector>

/** @brief Listener for ECS events. */
class ECSListener
{
public:
	// ECS events...

	/**
	 * Called any time the ECS makes an entity. If the listener is not listening to all entity
	 * operations, this will only be called if the entity contains all the components the listener 
	 * is interested in. Can be overridden in derived classes.
	 * 
	 * @param handle Handle to the newly created entity.
	 */
	virtual void OnMakeEntity(EntityHandle handle) {}

	/**
	 * Called any time the ECS is removing an entity. If the listener is not listening to all entity
	 * operations, this will only be called if the entity contains all the components the listener 
	 * is interested in. Specifically, this will be called right before the entity is removed from 
	 * the ECS. Can be overridden in derived classes.
	 * 
	 * @param handle Handle to entity being removed.
	 */
	virtual void OnRemoveEntity(EntityHandle handle) {}

	/**
	 * Called any time the ECS is adding a component. If the listener is not listening to all
	 * component operations, this will only be called if the component is of a type which the
	 * listener is interested in. Can be overridden in derived classes.
	 * 
	 * @param handle Handle to the entity which the component is attached to.
	 * @param id ID of the component type.
	 */
	virtual void OnAddComponent(EntityHandle handle, unsigned int id) {}

	/**
	 * Called any time the ECS is removing a component. If the listener is not listening to all
	 * component operations, this will only be called if the component is of a type which the 
	 * listener is interested in. Can be overridden in derived classes.
	 * 
	 * @param handle Handle to the entity which the component is attached to.
	 * @param id ID of the component type.
	 */
	virtual void OnRemoveComponent(EntityHandle handle, unsigned int id) {}

	/** @brief Gets the IDs of all component types which the listener is interesed in. */
	const std::vector<unsigned int>& GetComponentIDs() { return componentIDs; }

	/**
	 * @brief If false, component event methods should only be called if the component is of a type
	 * which the listener is interested in.
	 */
	inline bool ShouldNotifyOnAllComponentOperations() { return notifyOnAllComponentOperations; }

	/**
	 * @brief If false, entity event methods should only be called if the entity contains all
	 * components types the listener is interested in.
	 */
	inline bool ShouldNotifyOnAllEntityOperations() { return notifyOnAllEntityOperations; }

protected:
	/**
	 * Sets listener notification settings.
	 * 
	 * @param shouldNotifyOnAllComponentOperations If false, component event methods will only be
	 *		called if the component is of a type which the listener is interested in.
	 * 
	 * @param shouldNotifyOnAllEntityOperations If false, entity event methods will only be called
	 *		if the entity contains all components types the listener is interested in.
	 */
	void SetNotificationSettings(bool shouldNotifyOnAllComponentOperations,
		bool shouldNotifyOnAllEntityOperations)
	{
		notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
		notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
	}

	/**
	 * Adds a component type to the list of types which the listener should be interested in.
	 * 
	 * @param id ID of the component type to add.
	 */
	void AddComponentID(unsigned int id)
	{
		componentIDs.push_back(id);
	}

private:
	// The component types which the listener is interested in
	std::vector<unsigned int> componentIDs;
	bool notifyOnAllComponentOperations = false;
	bool notifyOnAllEntityOperations = false;
};

/**
 * @brief Entity Component System.
 * 
 * This class ties in together the entities, components, and systems together, which play an
 * important role in defining the logic of the game.
 * 
 * In short:
 * 
 * - A component is raw data. They contain no logic and are easy to isolate.
 * 
 * - A system is raw logic. A system takes a list of component types and performs any logic or 
 *   updates necessary to those specific components.
 * 
 * - An entity serves as a bridge between components and systems. An entity is an array of
 *   components which define an object in the game. Entities can only have one component of a given
 *   type (duplicate components are not allowed).
 * 
 * Internal versions of certain methods in ECS are used because the type name is not known at
 * compile time.
 */
class ECS
{
public:
	ECS() {}

	// A destructor is used because we are
	// - manually managing the memory of our components
	// - dynamically allocating and removing entities
	~ECS();

	// ECSListener methods

	/**
	 * Adds a listener to the ECS. Each listener's event methods will be called by the ECS on
	 * the corresponding events.
	 * 
	 * @see ECSListener
	 * 
	 * @param listener The listener to add.
	 */
	inline void AddListener(ECSListener* listener)
	{
		listeners.push_back(listener);
	}

	// Entity methods

	/**
	 * Entity constructor. Non-templated version.
	 * 
	 * @param entityComponents The components attached to the entity.
	 * @param componentIDs The types of the components being added.
	 * @param numberOfComponents The number of components being added.
	 * @return Handle to the newly created entity.
	 */
	EntityHandle MakeEntity(BaseECSComponent** entityComponents, const unsigned int* componentIDs,
		size_t numberOfComponents);

	/**
	 * Removes an entity from the ECS and deletes it.
	 * 
	 * @param handle Handle to the entity to delete.
	 */
	void RemoveEntity(EntityHandle handle);

	// Use variadic template parameters to allow any number of components to be passed in.
	template<class... Components>
	/**
	 * Entity constructor.
	 * 
	 * @param ...entityComponents The components attached to the entity.
	 * @return Handle to the newly created entity.
	 */
	EntityHandle MakeEntity(Components&... entityComponents)
	{
		BaseECSComponent* components[] = { (&entityComponents)... };

		// Or any function that removes references
		unsigned int componentIDs[] = { (std::remove_reference_t<Components>::ID)... };

		return MakeEntity(components, componentIDs, sizeof...(Components));
	}

	// Component methods

	template<class Component>
	/**
	 * Adds a component to an entity.
	 * 
	 * @param entity Handle to the entity which the component should be attached to.
	 * @param component Pointer to the component to attach.
	 */
	inline void AddComponent(EntityHandle entity, Component* component)
	{
		AddComponentInternal(HandleToEntity(entity), Component::ID, component);

		// Now that the component is created, dispatch the add component event to all listeners
		// in the ECS...

		// Iterate over all listeners
		for (unsigned int i = 0; i < listeners.size(); i++)
		{
			// Get the component types which the listener is interested in
			const std::vector<unsigned int>& componentIDs = listeners[i]->GetComponentIDs();

			// If the listener is interested in all component operations, dispatch the event
			// regardless of whether or not the listener is interested in the component type
			if (listeners[i]->ShouldNotifyOnAllComponentOperations())
			{
				listeners[i]->OnAddComponent(entity, Component::ID);
			}
			// Otherwise, check if the listener is interested in this component type
			else
			{
				// Iterate over all component types the listener is interested in
				for (unsigned int componentID : componentIDs)
				{
					// If the component type being added is one the component types the listener
					// is interested in
					if (componentID == Component::ID)
					{
						// Dispatch the event
						listeners[i]->OnAddComponent(entity, Component::ID);
						break;
					}
				}
			}
		}
	}

	template<class Component>
	/**
	 * Removes a component from an entity, based on component type.
	 * 
	 * @param entity Handle to the entity which the component will be removed from.
	 * @return If the component was sucessfully removed or not.
	 */
	inline bool RemoveComponent(EntityHandle entity)
	{
		// Before removing the component, dispatch the remove component event to all listeners
		// in the ECS...

		// Iterate over all listeners
		for (unsigned int i = 0; i < listeners.size(); i++)
		{
			// Get the component types which the listener is interested in
			const std::vector<unsigned int>& componentIDs = listeners[i]->GetComponentIDs();

			// If the listener is interested in all component operations, dispatch the event
			// regardless of whether or not the listener is interested in the component type
			if (listeners[i]->ShouldNotifyOnAllComponentOperations())
			{
				listeners[i]->OnRemoveComponent(entity, Component::ID);
			}
			// Otherwise, check if the listener is interested in this component type
			else
			{
				// Iterate over all component types the listener is interested in
				for (unsigned int componentID : componentIDs)
				{
					// If the component type being removed is one the component types the listener
					// is interested in
					if (componentID == Component::ID)
					{
						// Dispatch the event
						listeners[i]->OnRemoveComponent(entity, Component::ID);
						break;
					}
				}
			}
		}

		return RemoveComponentInternal(entity, Component::ID);
	}

	template<class Component>
	/**
	 * Gets a component from an entity, based on component type.
	 * 
	 * @param entity Handle to the entity to get the component from.
	 * 
	 * @return Pointer to the component, of the specified component type, belonging to the
	 *		specified entity.
	 */
	inline Component* GetComponent(EntityHandle entity)
	{
		return (Component*)GetComponentInternal(HandleToEntity(entity), components[Component::ID],
			Component::ID);
	}

	BaseECSComponent* GetComponentByType(EntityHandle entity, unsigned int componentID)
	{
		return GetComponentInternal(HandleToEntity(entity), components[componentID], componentID);
	}

	// System methods

	/**
	 * Updates all systems in a specified system list. Should be called every frame.
	 * 
	 * @param systems The systems to update.
	 * @param deltaTime How much time has passed since the previous update.
	 */
	void UpdateSystems(ECSSystemList& systems, float deltaTime);

private:
	// map<id, memory>
	std::unordered_map<unsigned int, std::vector<unsigned char>> components;

	// vector<pair<index, entity>>
	// The index is stored in the pair because it allows for efficient element removal
	//
	// An entity is an array of components; entity = vector<pair<componentType, componentIndex>>
	// where componentType is the ID and componentIndex is the index in components (defined above)
	std::vector<std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>*>
		entities;

	// All listeners which should have the appropriate methods called on ECS events
	std::vector<ECSListener*> listeners;

	/**
	 * Used internally for casting an EntityHandle to an index-entity pair pointer.
	 * 
	 * @param handle The entity handle.
	 * @return The index-entity pair pointer.
	 */
	inline std::pair<unsigned int, std::vector<std::pair<unsigned int,
		unsigned int>>>* HandleToRawType(EntityHandle handle)
	{
		return (std::pair<unsigned int, std::vector<std::pair<unsigned int,
			unsigned int>>>*)handle;
	}

	/**
	 * Used internally for getting the index of an entity in the entities vector/array, given an
	 * EntityHandle.
	 * 
	 * @param handle The entity handle.
	 * @return The index of the entity in the entities vector/array.
	 */
	inline unsigned int HandleToEntityIndex(EntityHandle handle)
	{
		return HandleToRawType(handle)->first;
	}

	/**
	 * Used internally for getting the entity for a given EntityHandle.
	 * 
	 * @param handle The entity handle.
	 * @return Reference to the entity. Note than an entity is an array of components.
	 */
	inline std::vector<std::pair<unsigned int, unsigned int>>& HandleToEntity(EntityHandle handle)
	{
		return HandleToRawType(handle)->second;
	}

	/**
	 * Used internally for deleting a component given a component type and index.
	 * 
	 * @param componentID The ID of the component type being deleted.
	 * @param index The index of the component in the components vector/array.
	 */
	void DeleteComponent(unsigned int componentID, unsigned int index);

	/**
	 * Used internally for removing a component from an entity.
	 * 
	 * @param handle The handle of the target entity.
	 * @param componentID ID of the component type being removed.
	 * @return If the component was successfully removed or not.
	 */
	bool RemoveComponentInternal(EntityHandle handle, unsigned int componentID);

	/**
	 * Used internally for adding a component to an entity. 
	 * This is the non-templated version which all templated methods call.
	 * 
	 * @param handle The handle of the target entity.
	 * @param entity Reference to the entity to attach the component to.
	 * @param componentID ID of the component type being added.
	 * @param component Pointer to the component being added.
	 */
	void AddComponentInternal(EntityHandle handle,
		std::vector<std::pair<unsigned int, unsigned int>>& entity, unsigned int componentID,
		BaseECSComponent* component);

	/**
	 * Used internally for getting a component from an entity, given a component type ID.
	 * 
	 * @param entityComponents The list of components attached to the entity.
	 * @param array Component type memory to lookup.
	 * @param componentID ID of the component type to get.
	 * @return Pointer to the requested component. Returns nullptr if the component is not found.
	 */
	BaseECSComponent* GetComponentInternal(
		std::vector<std::pair<unsigned int, unsigned int>>& entityComponents,
		std::vector<unsigned char>& array, unsigned int componentID);

	/**
	 * Used internally for updating a system with multiple components.
	 * 
	 * @param index Index of the system in the systems list to update.
	 * @param systems The systems list, used for looking up the system to update.
	 * @param deltaTime How much time has passed since the previous update.
	 * @param componentTypes The component types with which the system is working with.
	 * @param componentStorage Vector reference used to avoid repeatedly allocating and
	 *		deallocating every time the	function is called. Used for storing pointers to the 
	 *		components of an entity, which are passed on if the entity contains the non-optional
	 *		components with which the system is working with.
	 * @param componentArrays Vector reference used to avoid repeatedly allocating and
	 *		deallocating every time the	function is called. Used for storing the location of the
	 *		 memory/data for components.
	 */
	void UpdateSystemWithMultipleComponents(unsigned int index, ECSSystemList& systems,
		float deltaTime, const std::vector<unsigned int>& componentTypes,
		std::vector<BaseECSComponent*>& componentStorage,
		std::vector<std::vector<unsigned char>*>& componentArrays);

	/**
	 * Finds the least commonly occuring component from the list of component types specified. 
	 * Optional components will be ignored.
	 * 
	 * @param componentTypes The component types to check.
	 * @param componentFlags The flags for each component type specified.
	 * @see BaseECSSystem::FLAG_OPTIONAL
	 * @return The index, in the passed in array of component types, of the least commonly 
	 *		occuring component.
	 */
	unsigned int FindLeastCommonComponent(const std::vector<unsigned int>& componentTypes,
		const std::vector<unsigned int>& componentFlags);

	// Disallow copy and assign
	ECS(const ECS& other) = delete;
	ECS& operator=(const ECS& other) = delete;
};

