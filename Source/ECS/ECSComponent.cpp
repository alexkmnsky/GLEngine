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
