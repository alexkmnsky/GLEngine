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
#include <vector>

/**
 * @brief Base class which systems should inherit.
 * 
 * A system takes a certain set of components and updates every entity with those components.
 */
class BaseECSSystem
{
public:
	enum
	{
		// Allow for optional components; components that are processed only if they exist
		FLAG_OPTIONAL = 1
	};

	BaseECSSystem() {}

	/**
	 * Called every frame for each set of components with which the system is working with.
	 * 
	 * @param deltaTime How much time has passed since the previous update.
	 * @param components Array of components to update.
	 */
	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components) {}

	/**
	 * Gets the component types with which the system is working with.
	 * 
	 * @return Array of component IDs.
	 */
	const std::vector<unsigned int>& GetComponentTypes()
	{
		return componentTypes;
	}

	/**
	 * Gets the flags for each component type with which the system is working with.
	 * Indices correspond to those of the component types array.
	 * 
	 * @see GetComponentTypes
	 * @see FLAG_OPTIONAL
	 * 
	 * @return The list of flags for each component type.
	 */
	const std::vector<unsigned int>& GetComponentFlags()
	{
		return componentFlags;
	}

	/**
	 * Checks if a system is valid.
	 * Specifically, ensure that the system is working with at least one non-optional component.
	 * 
	 * @return If the system is valid or not.
	 */
	bool IsValid();

protected:
	/**
	 * Adds a component type to the system.
	 * 
	 * @param componentType The ID of the component type.
	 * @param componentFlag Any flag for the component. Defaults to no flag.
	 * 
	 * @see FLAG_OPTIONAL
	 */
	void AddComponentType(unsigned int componentType, unsigned int componentFlag = 0)
	{
		componentTypes.push_back(componentType);
		componentFlags.push_back(componentFlag);
	}

private:
	// The set of components with which a particular system is working with
	std::vector<unsigned int> componentTypes;
	std::vector<unsigned int> componentFlags;
};

class ECSSystemList
{
public:
	/**
	 * Adds a system to the list. The system will not be added if it is invalid.
	 * 
	 * @param system The system to add.
	 * @return If the system was successfully added or not.
	 */
	inline bool AddSystem(BaseECSSystem& system)
	{
		// If the system is invalid
		if (!system.IsValid())
			return false;

		// Add the system the list
		systems.push_back(&system);
		return true;
	}

	/** @brief Returns the number of elements in the system list. */
	inline size_t size()
	{
		return systems.size();
	}

	/** @brief Returns a pointer to the system at the specified index. */
	inline BaseECSSystem* operator[](unsigned int index)
	{
		return systems[index];
	}

	/**
	 * Removes a system from the list.
	 * 
	 * @param system The system to remove.
	 * @return If the system was successfully removed or not.
	 */
	bool RemoveSystem(BaseECSSystem& system);

private:
	std::vector<BaseECSSystem*> systems;
};
