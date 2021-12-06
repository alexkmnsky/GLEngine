#include "CollisionResponseInteraction.h"

void CollisionResponseInteraction::Interact(float deltaTime,
	EntityHandle interactor, EntityHandle interactee,
	BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents)
{
	// Get the interactor/interactee components
	TransformComponent* interactorTransform = ((TransformComponent*)interactorComponents[0]);
	TransformComponent* interacteeTransform = ((TransformComponent*)interacteeComponents[0]);
	ColliderComponent* interactorCollider = ((ColliderComponent*)interactorComponents[1]);
	ColliderComponent* interacteeCollider = ((ColliderComponent*)interacteeComponents[1]);
	MotionComponent* interactorMotion = ((MotionComponent*)interactorComponents[2]);

	// Ignore collision resolution if one of the two entities are set to overlap on collision
	if (interactorCollider->shouldOverlap || interacteeCollider->shouldOverlap)
		return;

	// Get the interactor's (entity with motion) position and velocity
	glm::vec3& interactorPosition = interactorTransform->transform.GetPosition();
	glm::vec3& interactorVelocity = interactorMotion->velocity;

	// Bounding box data
	glm::vec3 interactorMinExtents = interactorCollider->transformedAABB.GetMinExtents();
	glm::vec3 interactorMaxExtents = interactorCollider->transformedAABB.GetMaxExtents();
	glm::vec3 interacteeMinExtents = interacteeCollider->transformedAABB.GetMinExtents();
	glm::vec3 interacteeMaxExtents = interacteeCollider->transformedAABB.GetMaxExtents();


	// Find the distance between the objects on the near side
	glm::vec3 closestEdgeDistance;

	if (interactorVelocity.x > 0.0f)
		closestEdgeDistance.x = interacteeMinExtents.x - interactorMaxExtents.x;
	else
		closestEdgeDistance.x = interacteeMaxExtents.x - interactorMinExtents.x;

	if (interactorVelocity.y > 0.0f)
		closestEdgeDistance.y = interacteeMinExtents.y - interactorMaxExtents.y;
	else
		closestEdgeDistance.y = interacteeMaxExtents.y - interactorMinExtents.y;

	if (interactorVelocity.z > 0.0f)
		closestEdgeDistance.z = interacteeMinExtents.z - interactorMaxExtents.z;
	else
		closestEdgeDistance.z = interacteeMaxExtents.z - interactorMinExtents.z;

	// Find time of collision for each axis
	glm::vec3 entry;

	// If the value is zero, entry could not have occured
	// Set to infinity to avoid division by zero
	if (interactorVelocity.x == 0.0f)
		entry.x = -std::numeric_limits<float>::infinity();
	else
		entry.x = closestEdgeDistance.x / interactorVelocity.x;
	if (interactorVelocity.y == 0.0f)
		entry.y = -std::numeric_limits<float>::infinity();
	else
		entry.y = closestEdgeDistance.y / interactorVelocity.y;
	if (interactorVelocity.z == 0.0f)
		entry.z = -std::numeric_limits<float>::infinity();
	else
		entry.z = closestEdgeDistance.z / interactorVelocity.z;

	// Find the greatest entry time from each axis
	// This is also the entry time which will be used for determining the normal of the collision
	float entryTime = std::max(std::max(entry.x, entry.y), entry.z);

	glm::vec3 normal(0.0f);

	// entry.x is the greatest
	if (entry.x > entry.y && entry.x > entry.z)
		// If the interactor is moving positive on the x axis, then the face normal is towards the
		// negative x axis; otherwise it is the inverse
		normal = glm::vec3(interactorVelocity.x >= 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f);

	// entry.y is the greatest
	else if (entry.y > entry.x && entry.y > entry.z)
		// If the interactor is moving positive on the y axis, then the face normal is towards the
		// negative y axis; otherwise it is the inverse
		normal = glm::vec3(0.0f, interactorVelocity.y >= 0.0f ? -1.0f : 1.0f, 0.0f);

	// entry.z is the greatest
	else
		// If the interactor is moving positive on the z axis, then the face normal is towards the
		// negative z axis; otherwise it is the inverse
		normal = glm::vec3(0.0f, 0.0f, interactorVelocity.z >= 0.0f ? -1.0f : 1.0f);

	// Make sure the entry time is not infinity
	if (entryTime != std::numeric_limits<float>::infinity() &&
		entryTime != -std::numeric_limits<float>::infinity())
	{
		// If a collision response for the interactor entity does not already exist, initialize it
		// with the current collision response data
		if (!collisionResponses.count(interactor))
		{
			std::vector<std::pair<float, glm::vec3&>> possibleResponses;

			CollisionResponseData interactorData = { interactorPosition, interactorVelocity,
				 normal, entryTime };

			responseData.push_back(interactorData);

			collisionResponses.insert(
				std::pair<EntityHandle, CollisionResponseData*>(
					interactor, &responseData[responseData.size() - 1]));
		}
		// Otherwise, check if current entry time is smaller than the current collision response
		// entry time. If it is, then a more optimal collision response has been found.
		else if (entryTime < collisionResponses[interactor]->minEntryTime)
		{
			collisionResponses[interactor]->minEntryTime = entryTime;
			collisionResponses[interactor]->normal = normal;
		}
	}
}

void CollisionResponseInteraction::Finalize(float deltaTime)
{
	// Iterate over the collision responses for each entity
	for (auto& p : collisionResponses)
	{
		// Get struct values
		glm::vec3& position = p.second->position;
		glm::vec3& velocity = p.second->velocity;
		glm::vec3 normal = p.second->normal;
		float entryTime = p.second->minEntryTime;

		// Find the remaining time for the collision response
		float remainingTime = 1.0f - entryTime;

		// Resolve collision for the correct axis, and set the entity's velocity on the axis to zero
		if (normal.x)
		{
			position.x -= velocity.x * remainingTime * deltaTime;
			velocity.x = 0;
		}
		if (normal.y)
		{
			position.y -= velocity.y * remainingTime * deltaTime;
			velocity.y = 0;
		}
		if (normal.z)
		{
			position.z -= velocity.z * remainingTime * deltaTime;
			velocity.z = 0;
		}
	}

	// Clear all collision responses; they have been applied
	collisionResponses.clear();
}
