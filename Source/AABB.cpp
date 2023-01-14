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

#include "AABB.h"

AABB::AABB(const std::vector<glm::vec3>& points)
{
	// No points are specified; no bounding box can be formed
	if (points.size() == 0)
	{
		extents[0] = glm::vec3(0.0f);
		extents[1] = glm::vec3(0.0f);
		return;
	}

	// Set starting min and max
	extents[0] = points[0];
	extents[1] = points[0];

	// Loop over all specified points and find the min and max extents
	for (const glm::vec3& point : points)
	{
		extents[0].x = std::min(extents[0].x, point.x);
		extents[0].y = std::min(extents[0].y, point.y);
		extents[0].z = std::min(extents[0].z, point.z);

		extents[1].x = std::max(extents[1].x, point.x);
		extents[1].y = std::max(extents[1].y, point.y);
		extents[1].z = std::max(extents[1].z, point.z);
	}
}

bool AABB::Intersects(const AABB& other) const
{
	return !(
		(
			extents[0].x >= other.extents[1].x ||
			extents[0].y >= other.extents[1].y ||
			extents[0].z >= other.extents[1].z
		) || (
			extents[1].x <= other.extents[0].x ||
			extents[1].y <= other.extents[0].y ||
			extents[1].z <= other.extents[0].z
		)
	);
}


AABB AABB::Translate(const glm::vec3& translation) const
{
	// Apply translation to min and max extents
	return AABB(extents[0] + translation, extents[1] + translation);
}
