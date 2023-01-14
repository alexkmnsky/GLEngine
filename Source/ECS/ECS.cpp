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

#include "ECS.h"
#include <iostream>
#include <algorithm> // std::max
#include <cstring> // std::memcpy

ECS::~ECS()
{
	// Iterate over all component types, and delete every component
	for (auto it = components.begin(); it != components.end(); it++)
	{
		// The first element of the pair is the component type ID
		// The second element of the pair is the arbitrary memory for the component type

		// Find the memory size of the component type
		size_t typeSize = BaseECSComponent::GetTypeSize(it->first);

		// Get the component free function, which defines how to free the component at runtime
		ECSComponentFreeFunction freeFunction = BaseECSComponent::GetTypeFreeFunction(it->first);

		// Iterate through the memory of the component
		// Increment by type size; every component of the same type has the same size
		// This has the result of iterating over every single component
		for (unsigned int i = 0; i < it->second.size(); i+= typeSize)
		{
			// Call the free function on each component
			// Casting to BaseECSComponent is safe; all components inherit from BaseECSComponent
			freeFunction((BaseECSComponent*)&it->second[i]);
		}
	}

	// Iterate over all entities and delete them
	for (auto& entity : entities)
	{
		delete entity;
	}
}

EntityHandle ECS::MakeEntity(BaseECSComponent** entityComponents, const unsigned int* componentIDs,
	size_t numberOfComponents)
{
	// Initialize a new entity
	std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>* newEntity =
		new std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>();

	// Create an entity handle for the new entity
	EntityHandle handle = (EntityHandle)newEntity;

	// Iterate over all components specified and add them to the new entity
	for (unsigned int i = 0; i < numberOfComponents; i++)
	{
		// Check if componentID is actually valid
		if (!BaseECSComponent::IsTypeValid(componentIDs[i]))
		{
			std::cerr << "'" << componentIDs[i] << "' is not a valid component type." << std::endl;
			// Abort; delete the new entity and return a null entity handle
			delete newEntity;
			return nullptr;
		}

		AddComponentInternal(handle, newEntity->second, componentIDs[i], entityComponents[i]);
	}

	// The first element of the entity pair is it's index in the entities vector/array
	// See declaration of entities for more info
	newEntity->first = entities.size();

	// Add the new entity to the entities vector/array
	entities.push_back(newEntity);

	// Now that the entity is created, dispatch the make entity event to all listeners
	// in the ECS...

	std::vector<std::pair<unsigned int, unsigned int>>& entity = HandleToEntity(handle);

	// Iterate over all listeners
	for (unsigned int i = 0; i < listeners.size(); i++)
	{
		// Get the component types which the listener is interested in
		const std::vector<unsigned int>& listenerComponentIDs = listeners[i]->GetComponentIDs();
		bool isValid = true;

		// If the listener is interested in all entity operations, dispatch the event regardless
		// of whether or not the listener is interested in the entity's component types
		if (listeners[i]->ShouldNotifyOnAllEntityOperations())
		{
			listeners[i]->OnMakeEntity(handle);
		}
		// Otherwise, check if the entity contains all component types which the listener is
		// interested in
		else
		{
			// Iterate over all component types the listener is interested in
			for (unsigned int j = 0; j < listenerComponentIDs.size(); j++)
			{
				bool hasComponent = false;
				// Iterate over all components in the entity
				for (unsigned int k = 0; k < numberOfComponents; k++)
				{
					// If the component type is the component type which the listener is
					// interested in
					if (listeners[i]->GetComponentIDs()[j] == componentIDs[k])
					{
						// The entity has this particular component
						hasComponent = true;
						// Stop searching
						break;
					}
				}
				// If the entity does not have this component type, it does not have all component
				// types which the listener is interested in
				if (!hasComponent)
				{
					isValid = false;
					// Stop searching; the entity can no longer be valid
					break;
				}
			}
			// If the entity has all component types which the listener is interested in
			if (isValid)
			{
				// Dispatch the event
				listeners[i]->OnMakeEntity(handle);
			}
		}
	}

	return handle;
}

void ECS::RemoveEntity(EntityHandle handle)
{
	std::vector<std::pair<unsigned int, unsigned int>>& entity = HandleToEntity(handle);

	// Before removing the entity, dispatch the remove entity event to all listeners
	// in the ECS...

	// Iterate over all listeners
	for (unsigned int i = 0; i < listeners.size(); i++)
	{
		// Get the component types which the listener is interested in
		const std::vector<unsigned int>& listenerComponentIDs = listeners[i]->GetComponentIDs();
		bool isValid = true;

		// If the listener is interested in all entity operations, dispatch the event regardless
		// of whether or not the listener is interested in the entity's component types
		if (listeners[i]->ShouldNotifyOnAllEntityOperations())
		{
			listeners[i]->OnRemoveEntity(handle);
		}
		// Otherwise, check if the entity contains all component types which the listener is
		// interested in
		else
		{
			// Iterate over all component types the listener is interested in
			for (unsigned int j = 0; j < listenerComponentIDs.size(); j++)
			{
				bool hasComponent = false;
				// Iterate over all components in the entity
				for (unsigned int k = 0; k < entity.size(); k++)
				{
					// If the component type is the component type which the listener is
					// interested in
					if (listenerComponentIDs[j] == entity[k].first)
					{
						// The entity has this particular component
						hasComponent = true;
						// Stop searching
						break;
					}
				}
				// If the entity does not have this component type, it does not have all component
				// types which the listener is interested in
				if (!hasComponent)
				{
					isValid = false;
					// Stop searching; the entity can no longer be valid
					break;
				}
			}
			// If the entity has all component types which the listener is interested in
			if (isValid)
			{
				// Dispatch the event
				listeners[i]->OnRemoveEntity(handle);
			}
		}
	}

	// Proceed with removing the entity...

	// Make sure all components attached to the entity are deleted
	for (unsigned int i = 0; i < entity.size(); i++)
	{
		DeleteComponent(entity[i].first, entity[i].second);
	}

	// All components are gone; the entity can now be safely removed from the entities list...
	//
	// The naive way of removing the entity would be to remove it from the vector/array and
	// to adjust it accordingly.
	//
	// However, because the order of the entities list is irrelevant, we can move the
	// entity at the end of the list to the index of the entity being deleted. All that matters is
	// that the entities list contains every entity. Performing this bypasses the need to shift
	// elements of the array.

	// The current index; the index of the entity being removed in the entities list
	unsigned int destinationIndex = HandleToEntityIndex(handle);

	// The index of the last entity in the entities list
	unsigned int sourceIndex = entities.size() - 1;

	// Delete the entity being removed
	delete entities[destinationIndex];

	// Copy the last entity in the list to where the entity being deleted is
	entities[destinationIndex] = entities[sourceIndex];
	
	// Update the index of the entity in the array
	entities[destinationIndex]->first = destinationIndex;

	// Remove the last element of the entities list
	entities.pop_back();
}

void ECS::UpdateSystems(ECSSystemList& systems, float deltaTime)
{
	// Created in advance to avoid repeatedly allocating and deallocating for every
	// single system updated; passed in by reference to UpdateSystemWithMultipleComponents
	std::vector<BaseECSComponent*> componentStorage;
	std::vector<std::vector<unsigned char>*> componentArrays;

	// Iterate over all systems in the system list
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		const std::vector<unsigned int>& componentTypes = systems[i]->GetComponentTypes();

		// If the system has only one component type
		if (componentTypes.size() == 1)
		{
			// Find the memory size of the component type
			size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[0]);

			// Get the component type's memory array by reference
			std::vector<unsigned char>& array = components[componentTypes[0]];

			// Iterate through the memory of the component
			// Increment by type size; every component of the same type has the same size
			// This has the result of iterating over every single component
			for (unsigned int j = 0; j < array.size(); j += typeSize)
			{
				// Casting to BaseECSComponent is safe; all components inherit from BaseECSComponent
				BaseECSComponent* component = (BaseECSComponent*)&array[j];

				// Update the component
				systems[i]->UpdateComponents(deltaTime, &component);
			}
		}
		else
		{
			// Handle the more difficult case of updating a system with multiple components
			UpdateSystemWithMultipleComponents(i, systems, deltaTime, componentTypes,
				componentStorage, componentArrays);
		}
	}
}

void ECS::UpdateSystemWithMultipleComponents(unsigned int index, ECSSystemList& systems,
	float deltaTime, const std::vector<unsigned int>& componentTypes,
	std::vector<BaseECSComponent*>& componentStorage,
	std::vector<std::vector<unsigned char>*>& componentArrays)
{
	const std::vector<unsigned int>& componentFlags = systems[index]->GetComponentFlags();

	// If the arrays are not large enough for all component types, resize them to the number of
	// component types
	componentStorage.resize(std::max(componentStorage.size(), componentTypes.size()));
	componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));

	// Iterate through all component types with which the system is working with
	for (unsigned int i = 0; i < componentTypes.size(); i++)
		// Save the location of the memory/data of the component
		// This will reduce the number of times we have to lookup the component in the map
		// (as we would otherwise be performing the lookup in a double for loop)
		componentArrays[i] = &components[componentTypes[i]];

	// Find the least commonly occuring component type from the component types the system is 
	// working with. This component type will be used to look up all possible entities which
	// could have all the components with which the system is working with. Using the least 
	// commonly occuring component will reduce the number of entities which we check.
	unsigned int minSizeIndex = FindLeastCommonComponent(componentTypes, componentFlags);

	// Find the memory size of the least common component type
	size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[minSizeIndex]);

	// Get the memory array for the component type, containing all components of that type
	std::vector<unsigned char>& array = *componentArrays[minSizeIndex];

	// Iterate through the memory of the component type
	// Increment by type size; every component of the same type has the same size
	// This has the result of iterating over every single component
	for (unsigned int i = 0; i < array.size(); i += typeSize)
	{
		// Save the component in the array
		// The array of components will be passed into UpdateComponents if this entity contains
		// all component types with which the system is working with
		componentStorage[minSizeIndex] = (BaseECSComponent*)&array[i];

		// Get the entity which the component at this index is attached to
		// The entity contains all other component types attached
		std::vector<std::pair<unsigned int, unsigned int>>& entityComponents =
			HandleToEntity(componentStorage[minSizeIndex]->entity);

		// Check if the entity is missing one of the components the system is working with...
		// If the entity is missing one of the components, then it should be ignored

		// Does the entity have all components
		bool isValid = true;

		// Iterate over all components in the entity
		for (unsigned int j = 0; j < componentTypes.size(); j++)
		{
			// Special case where it is already known that the component exists in this entity 
			// because it was used to retrieve the entity
			if (j == minSizeIndex)
			{
				continue;
			}

			// Save the component in the array
			componentStorage[j] = GetComponentInternal(entityComponents, *componentArrays[j],
				componentTypes[j]);

			// If GetComponentInternal returned nullptr, then the component was not found in
			// the entity
			// If the component type is not optional in this system, then this entity is not valid
			// and should be ignored
			if (componentStorage[j] == nullptr &&
				((componentFlags[j] & BaseECSSystem::FLAG_OPTIONAL) == 0))
			{
				isValid = false;
				// The entity can no longer be valid; the rest of the components do not need
				// to be checked
				break;
			}
		}

		// If the entity has all components
		if (isValid)
			// Call UpdateComponents on the system
			// Pass in the array of components to update
			systems[index]->UpdateComponents(deltaTime, componentStorage.data());
	}
}

unsigned int ECS::FindLeastCommonComponent(const std::vector<unsigned int>& componentTypes,
	const std::vector<unsigned int>& componentFlags)
{
	// Set to integer limit; highest value possible; we are looking for the minimum value
	unsigned int minSize = (unsigned int)-1;
	unsigned int minIndex = (unsigned int)-1;

	// Iterate through all component types passed in
	for (unsigned int i = 0; i < componentTypes.size(); i++)
	{
		// If the component type is optional, ignore it
		if ((componentFlags[i] & BaseECSSystem::FLAG_OPTIONAL) != 0)
			continue;

		// Find the memory size of the component type
		size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[i]);

		// Get the size of the memory array for the component type (bytes) and divide it by
		// the size of the component type (bytes)
		// This finds how many times the component occurs, or in other words, how common the
		// component is
		unsigned int size = components[componentTypes[i]].size() / typeSize;
		// If this component type is less common, set the minSize and minIndex accordingly
		if (size <= minSize)
		{
			minSize = size;
			minIndex = i;
		}
	}

	return minIndex;
}

void ECS::DeleteComponent(unsigned int componentID, unsigned int index)
{
	// Component type memory/data
	std::vector<unsigned char>& array = components[componentID];

	// Get the component type's free function, which defines how to free the component at runtime
	ECSComponentFreeFunction freeFunction = BaseECSComponent::GetTypeFreeFunction(componentID);

	// Get the component type's size, which will be used for deallocating the correct amount
	// of memory from the component type's memory array
	size_t typeSize = BaseECSComponent::GetTypeSize(componentID);

	// Index of the last element in array
	unsigned int sourceIndex = array.size() - typeSize;

	// Cast the memory to BaseECSComponent pointers
	// This is a safe cast because all elements in the array are components that inherit from
	// BaseECSComponent
	BaseECSComponent* destinationComponent = (BaseECSComponent*)&array[index];
	BaseECSComponent* sourceComponent = (BaseECSComponent*)&array[sourceIndex];

	// Call the free function on the component being deleted
	freeFunction(destinationComponent);

	// Special case where the component being deleted is the final element of the array
	if (index == sourceIndex)
	{
		array.resize(sourceIndex);
		return;
	}

	// Copy the last element of the array to the location of the deleted component
	std::memcpy(destinationComponent, sourceComponent, typeSize);

	// Note that entities store components as a <type ID, index> pair, where the index represents 
	// the location of the component in the component type's memory array. This means that if we 
	// move the location of a component, we must also update the index in the entity.

	// Get a reference to the entity which the component being copied is attached to
	std::vector<std::pair<unsigned int, unsigned int>>& sourceComponents =
		HandleToEntity(sourceComponent->entity);

	// Search for the source component in the entity...
	// Iterate over all components in the entity
	for (unsigned int i = 0; i < sourceComponents.size(); i++)
	{
		// If we found the source component
		if (componentID == sourceComponents[i].first && sourceIndex == sourceComponents[i].second)
		{
			// Update the index of the component in the component type's memory array
			sourceComponents[i].second = index;
			break;
		}
	}

	// Remove the last element from the array
	array.resize(sourceIndex);
}

bool ECS::RemoveComponentInternal(EntityHandle handle, unsigned int componentID)
{
	// Get a reference to the entity which the component is being removed from
	std::vector<std::pair<unsigned int, unsigned int>>& entityComponents = HandleToEntity(handle);

	// Iterate over all components in the entity
	for (unsigned int i = 0; i < entityComponents.size(); i++)
	{
		// If the component ID matches the ID of the component to remove
		if (componentID == entityComponents[i].first)
		{
			// Delete the component
			// Note that this only deletes the component at it's index in the components array
			// The component <type ID, index> pair still needs to be removed from the entity
			DeleteComponent(entityComponents[i].first, entityComponents[i].second);

			// Swap the component being removed with the final component in the vector/array,
			// and then delete the final element
			unsigned int sourceIndex = entityComponents.size() - 1;
			unsigned int destinationIndex = i;
			entityComponents[destinationIndex] = entityComponents[sourceIndex];
			entityComponents.pop_back();

			// Successfully removed the component from the entity
			return true;
		}
	}

	// The component was not removed; the component ID was not found in the entity
	return false;
}

void ECS::AddComponentInternal(EntityHandle handle, 
	std::vector<std::pair<unsigned int,	unsigned int>>& entity, unsigned int componentID,
	BaseECSComponent* component)
{
	// Get the component type's create function, which defines how to create the component
	// at runtime
	ECSComponentCreateFunction createFunction = BaseECSComponent::GetTypeCreateFunction(componentID);

	// pair<component ID, index in components vector/array>
	std::pair<unsigned int, unsigned int> newPair;

	newPair.first = componentID;

	// The create function returns the location/index of the component created in the memory array
	// See ECSComponentCreate
	//
	// Pass in the components memory array, which is where we want the component to be created,
	// the entity handle, so that the component knows what entity it belongs to, and the pointer
	// to the component to create
	newPair.second = createFunction(components[componentID], handle, component);

	// Add the newly created component to our entity
	entity.push_back(newPair);
}

BaseECSComponent* ECS::GetComponentInternal(
	std::vector<std::pair<unsigned int, unsigned int>>& entityComponents,
	std::vector<unsigned char>& array, unsigned int componentID)
{
	// Iterate over all components in the entity
	for (unsigned int i = 0; i < entityComponents.size(); i++)
	{
		// If the component ID matches the ID of the component to get
		if (componentID == entityComponents[i].first)
		{
			// Retrieve and return the component
			// This is a safe cast because all elements in the array are components that inherit
			// from BaseECSComponent
			return (BaseECSComponent*)&array[entityComponents[i].second];
		}
	}
	// The component is not found
	return nullptr;
}