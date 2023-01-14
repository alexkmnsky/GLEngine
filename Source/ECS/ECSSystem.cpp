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
