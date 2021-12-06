#include "ECSComponent.h"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>*
	BaseECSComponent::componentTypes;

unsigned int BaseECSComponent::RegisterComponentType(ECSComponentCreateFunction createFunction, 
	ECSComponentFreeFunction freeFunction, size_t size)
{
	// If the component types array has not been initialized
	if (componentTypes == nullptr)
	{
		componentTypes = new std::vector<std::tuple<ECSComponentCreateFunction,
			ECSComponentFreeFunction, size_t>>();
	}

	// The ID starts at zero.
	// Given the number of registered components, we can find the next available ID by using
	// the size of componentTypes.
	unsigned int componentID = componentTypes->size();
	
	// Add the component's parameters to the lookup array
	componentTypes->push_back(std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction,
		size_t>(createFunction, freeFunction, size));

	return componentID;
}
