#include "ECSSystem.h"

bool BaseECSSystem::IsValid()
{
	// Iterate over all component flags
	for (unsigned int i = 0; i < componentFlags.size(); i++)
	{
		// If the component is not optional
		if ((componentFlags[i] & BaseECSSystem::FLAG_OPTIONAL) == 0)
		{
			// The system is valid
			return true;
		}
	}
	// The system is not valid; either it is working with no components, or all components are
	// optional
	return false;
}


bool ECSSystemList::RemoveSystem(BaseECSSystem& system)
{
	// Iterate over all systems in the systems list
	for (unsigned int i = 0; i < systems.size(); i++)
	{
		// If the system matches the system to remove
		if (&system == systems[i])
		{
			// Remove the system from the list
			systems.erase(systems.begin() + i);

			// The system was successfully removed
			return true;
		}
	}
	// The system was not removed; it was not found in the list
	return false;
}
