#pragma once

#include <GLM/glm.hpp>

struct PhysicsObject
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
    float mass;
};