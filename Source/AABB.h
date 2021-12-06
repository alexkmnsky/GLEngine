#pragma once

#include <vector>
#include <algorithm>
#include <GLM/glm.hpp>

/** @brief Axis-aligned bounding box. Used for collision detection. */
class AABB
{
public:
	/** @brief Default AABB constructor. */
	inline AABB()
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
	inline AABB(const glm::vec3& minExtents, const glm::vec3& maxExtents)
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
	AABB(std::vector<glm::vec3>& points);

	/**
	 * Determines if the AABB is intersecting another AABB.
	 * 
	 * @param other The other AABB to compare against.
	 * @return Whether or not the two AABBs are intersecting.
	 */
	bool Intersects(const AABB& other);

	/**
	 * Translates a copy of the AABB. No rotation or scaling is applied.
	 * 
	 * @param translation The translation to apply.
	 * @return The translated copy of the AABB.
	 */
	AABB Translate(const glm::vec3& translation) const;

	// Getter methods...
	inline glm::vec3 GetCenter() { return (extents[0] + extents[1]) * 0.5f; }
	inline glm::vec3 GetMinExtents() { return extents[0]; }
	inline glm::vec3 GetMaxExtents() { return extents[1]; }

private:
	glm::vec3 extents[2];
};