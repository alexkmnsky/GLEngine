#pragma once

#include "AABB.h"

#include <unordered_map>
#include <GLM/glm.hpp>
#include <cassert>
#include <iostream>

namespace Algorithm
{
	/**
	 * @brief Octree data structure. Used for efficient space partitioning.
	 * @tparam Handle Handle used for uniquely identifying bounding boxes.
	 */
	template<typename Handle>
	class Octree
	{
	public:
		/**
		 * @param boundingBox Represents the bounds of the octree node
		 */
		Octree(const AABB& boundingBox) : boundingBox(boundingBox) {}
		virtual ~Octree() = default;

		/**
		 * @brief Performs insertion of some object so that it can be kept track of by the octree.
		 * @param handle Uniquely identifying handle for the object being inserted. This is what
		 *		will be used to search for the object in other methods.
		 * @param objectBounds The bounding box of the object being inserted.
		 */
		void Insert(const Handle& handle, const AABB& objectBounds)
		{
			// This node already has children
			if (hasSplit)
			{
				if (!InsertToChildren(handle, objectBounds))
				{
					objects.insert({ handle, objectBounds });
				}
			}
			// If this node is a leaf node
			else
			{
				objects.insert({ handle, objectBounds });
				if (objects.size() > MAX_CAPACITY)
				{
					Split();
				}
			}
		}

		/**
		 * @brief Checks whether or not two objects are in a similar region of the octree.
		 * @param a The first object to compare.
		 * @param b The second object to compare.
		 * @return true if the two objects are in a similar region of the octree.
		 * @see AreRelatedInternal
		 */
		[[nodiscard]] bool AreRelated(const Handle& a, const Handle& b)
		{
			return AreRelatedInternal(a, b, false, false);
		}

	private:
		static constexpr unsigned int MAX_CAPACITY = 5;

		/**
		 * @brief Creates 8 octree children.
		 */
		void Split()
		{
			hasSplit = true;

			const glm::vec3 center = boundingBox.GetCenter();
			const glm::vec3 minExtents = boundingBox.GetMinExtents();
			const glm::vec3 maxExtents = boundingBox.GetMaxExtents();

			// Reserve vector size for each of the 8 subdivisions
			children.reserve(8);

			// Loops over every vertex of the bounding box which are used to generate split bounding
			// boxes
			for (const float x : { minExtents.x, maxExtents.x })
			{
				for (const float y : { minExtents.y, maxExtents.y })
				{
					for (const float z : {minExtents.z, maxExtents.z})
					{
						children.emplace_back(AABB(std::vector{ glm::vec3(x, y, z), center }));
					}
				}
			}

			// Reinsert all the objects into the child nodes
			for (auto it = objects.cbegin(); it != objects.cend(); )
			{
				if (InsertToChildren(it->first, it->second))
				{
					it = objects.erase(it);
				}
				else
				{
					it = std::next(it);
				}
			}
		}

		/**
		 * @brief Inserts an object into the octree given that the octree has been previously split.
		 * @param handle Uniquely identifying handle for the object being inserted. This is what
		 *		will be used to search for the object in other methods.
		 * @param objectBounds The bounding box of the object being inserted.
		 * @return Returns whether or not a child was found that completely encompasses the object.
		 */
		bool InsertToChildren(const Handle& handle, const AABB& objectBounds)
		{
			assert(hasSplit);

			unsigned int numIntersections = 0;
			unsigned int subtreeIndex = 0;

			// Loop over all child octrees
			for (unsigned int i = 0; i < 8; i++)
			{
				// If we find a subtree which contains the bounding box
				if (objectBounds.Intersects(children[i].boundingBox))
				{
					numIntersections++;
					// If there is more than one child which overlaps the bounding box, add it
					// to the parent
					if (numIntersections > 1)
					{
						return false;
					}
					subtreeIndex = i;
				}
			}

			// Object is outside of the octree?
			assert(numIntersections != 0);

			// If the bounding box entirely fits in one child, insert the bounding box into that
			// child and remove it from the parent if it exits
			children[subtreeIndex].Insert(handle, objectBounds);
			return true;
		}

		/**
		 * @brief Used internally for checking if two objects are in a similar region of the octree.
		 * @param a The first object to compare.
		 * @param b The second object to compare.
		 * @param aFound Indicates whether or not a has been found in a parent octree.
		 * @param bFound Indicates whether or not b has been found in a parent octree.
		 * @return true if the two objects are in a similar region of the octree.
		 * @see AreRelated
		 */
		[[nodiscard]] bool AreRelatedInternal(const Handle& a, const Handle& b, bool aFound,
			bool bFound) const
		{
			const bool containsA = objects.count(a);
			const bool containsB = objects.count(b);

			// If this node contains both handle's they are definitely related
			if (containsA && containsB) return true;

			// If we previously found handle b and just found handle a, a and b are related
			if (containsA && bFound) return true;

			// If we previously found handle a and just found handle b, a and b are related
			if (containsB && aFound) return true;

			// If the current node contains a we should look for b
			if (containsA)
			{
				for (const Octree& child : children)
				{
					if (child.AreRelatedInternal(a, b, true, bFound)) return true;
				}
			}
			// If the current node contains b we should look for a
			else if (containsB)
			{
				for (const Octree& child : children)
				{
					if (child.AreRelatedInternal(a, b, aFound, true)) return true;
				}
			}
			// Recursively search the child nodes for a and b
			else
			{
				for (const Octree& child : children)
				{
					if (child.AreRelatedInternal(a, b, aFound, bFound)) return true;
				}
			}

			return false;
		}

		bool hasSplit = false;
		std::unordered_map<Handle, AABB> objects;
		std::vector<Octree> children;
		const AABB boundingBox;
	};
}
