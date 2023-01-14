/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#pragma once

#include "ECS/ECSComponent.h"
#include <GLM/glm.hpp>

struct RigidbodyComponent : ECSComponent<RigidbodyComponent>
{
    glm::vec3 force;
    glm::vec3 velocity;

    float mass;
    bool takesGravity;

    float staticFriction;
    float dynamicFriction;
    float restitution;
};