#pragma once

#include <GLM/glm.hpp>

/**
 * @brief Motion integrators determine how to update an object's position/velocity/acceleration.
 * Using an integrator performs the updates in a mathematically sound fashion.
 */
namespace MotionIntegrators
{
	/**
	 * Modified Euler motion integrator.
	 * 
	 * @param position Position to update.
	 * @param velocity Velocity to update.
	 * @param acceleration Acceleration, used for adjusting velocity.
	 * @param deltaTime How much time to integrate over.
	 */
	inline void ModifiedEuler(glm::vec3& position, glm::vec3& velocity,
		const glm::vec3& acceleration, float deltaTime)
	{
		// Add how much the object should be accelerating, multiplied by how much time has passed
		velocity += acceleration * deltaTime;

		// Update the position of the object based on it's velocity, multiplied by how much time
		// has passed
		position += velocity * deltaTime;
	}
}