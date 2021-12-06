#pragma once

#include <tuple>
#include <vector>
#include <utility>

// Forward declaration of the struct for the typedefs below
struct BaseECSComponent;

/** @brief Used for refering to any entity. */
typedef void* EntityHandle;

/** @brief Pointer to the component create function. */
typedef unsigned int(*ECSComponentCreateFunction)(std::vector<unsigned char>& memory,
	EntityHandle entity, BaseECSComponent* component);

/** @brief Pointer to the component free function. */
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* component);

/** @brief Base struct which components derive from. */
struct BaseECSComponent
{
public:
	/**
	 * Registers a component's create function, free function, and size for future lookup. 
	 * Determines the next available ID for the component type, and returns it.
	 * 
	 * @param createFunction Pointer to the component create function, which defines how to create
	 *		the component at runtime.
	 * 
	 * @param freeFunction Pointer to the component free function, which defines how to free the 
	 *		component at runtime.
	 * 
	 * @param size The size of the component type in bytes.
	 * 
	 * @return The component ID of the newly registered component type.
	 */
	static unsigned int RegisterComponentType(ECSComponentCreateFunction createFunction,
		ECSComponentFreeFunction freeFunction, size_t size);

	// The entity which the component is attached to
	EntityHandle entity = nullptr;

	/**
	 * Gets the component create function for a given component type.
	 * 
	 * @param id The ID of the component type to look up.
	 * @return The create function.
	 */
	inline static ECSComponentCreateFunction GetTypeCreateFunction(unsigned int id)
	{
		return std::get<0>((*componentTypes)[id]);
	}

	/**
	 * Gets the component free function for a given component type.
	 *
	 * @param id The ID of the component type to look up.
	 * @return The free function.
	 */
	inline static ECSComponentFreeFunction GetTypeFreeFunction(unsigned int id)
	{
		return std::get<1>((*componentTypes)[id]);
	}

	/**
	 * Gets the size of a given component type.
	 *
	 * @param id The ID of the component type to look up.
	 * @return The size in bytes.
	 */
	inline static size_t GetTypeSize(unsigned int id)
	{
		return std::get<2>((*componentTypes)[id]);
	}

	/**
	 * @brief Determines if a component type ID is within the range of registered component types.
	 */
	inline static bool IsTypeValid(unsigned int id)
	{
		return id < componentTypes->size();
	}

private:
	// Lookup array for all the parameters of a given component type, index corresponds to
	// component ID. This gives us a global index of all component types at runtime.
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction,
		size_t>>* componentTypes;
};

template<typename T>
/**
 * @brief Intermediary ECS component struct which components should inherit.
 * 
 * This struct holds the static variables for components. They are templated for every individual
 * type of component because the component is the template type. This eliminates the need of having
 * all the static fields in every single component.
 */
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;
	static const unsigned int ID;
	static const size_t SIZE;
};


template<typename Component>
/**
 * This function defines how to create a component at runtime.
 *
 * @param memory Byte array of memory.
 * @param entity The entity handle of the entity which this component should be attached to.
 * @param component The component to initialize.
 * @return Location/index of the component created in the memory array.
 */
unsigned int ECSComponentCreate(std::vector<unsigned char>& memory, EntityHandle entity,
	BaseECSComponent* component)
{
	// The component will be added at the end of the memory array
	unsigned int index = memory.size();

	// Resize the memory array to include the space needed to store the component
	memory.resize(index + Component::SIZE);

	// Construct the new component. Specify the address of memory[index] in order for the new
	// component to be constructed in the memory array.
	Component* c = new(&memory[index]) Component(*(Component*)component);

	// Set the entity which the component is attached to
	c->entity = entity;

	return index;
}

template<typename Component>
/**
 * This function defines how to free a component at runtime.
 *
 * @param component The component to free.
 */
void ECSComponentFree(BaseECSComponent* component)
{
	Component* c = (Component*)component;

	// Manually call the destructor
	c->~Component();
}

template<typename T>
// Set the component ID for every individual component
// RegisterComponentType returns the ID of the new component
const unsigned int ECSComponent<T>::ID(BaseECSComponent::RegisterComponentType(
	ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
// Knowing the size of an individual component helps us with dynamically allocating memory for  
// a component of a particular type
const size_t ECSComponent<T>::SIZE(sizeof(T));

// Funcions for dynamic memory allocation at runtime...
template<typename T>
// How to create the component at runtime
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
// How to free the component at runtime
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);