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

#include <vector>
#include <algorithm>
#include <GLM/glm.hpp>

/** @brief Axis-aligned bounding box. Used for collision detection. */
class AABB
{
public:
	/** @brief Default AABB constructor. */
	AABB()
	{
		extents[0] = glm::vec3(0.0f);
		extents[1] = glm::vec3(0.0f);
	}

	/**
	 * Creates an AABB knowing the min and max extents.
	 * Only two points are needed for defining an axis-aligned box.
	 * 
	 * @param minExtents
	 * @param maxExtents
	 */
	AABB(const glm::vec3& minExtents, const glm::vec3& maxExtents)
	{
		extents[0] = minExtents;
		extents[1] = maxExtents;
	}

	/**
	 * Creates an AABB given a list of points. This will loop over all specified points and
	 * find the min and max extents. This is useful for determining the AABB of a mesh or any other
	 * object with many vertices.
	 * 
	 * @param points The points to loop over.
	 */
	AABB(const std::vector<glm::vec3>& points);

	/**
	 * Determines if the AABB is intersecting another AABB.
	 * 
	 * @param other The other AABB to compare against.
	 * @return Whether or not the two AABBs are intersecting.
	 */
	bool Intersects(const AABB& other) const;

	/**
	 * Translates a copy of the AABB. No rotation or scaling is applied.
	 * 
	 * @param translation The translation to apply.
	 * @return The translated copy of the AABB.
	 */
	[[nodiscard]] AABB Translate(const glm::vec3& translation) const;

	// Getter methods...
	[[nodiscard]] glm::vec3 GetCenter() const { return (extents[0] + extents[1]) * 0.5f; }
	[[nodiscard]] glm::vec3 GetMinExtents() const { return extents[0]; }
	[[nodiscard]] glm::vec3 GetMaxExtents() const { return extents[1]; }

private:
	glm::vec3 extents[2];
};