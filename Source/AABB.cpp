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
