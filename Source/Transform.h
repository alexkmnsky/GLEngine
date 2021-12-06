#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3& position = glm::vec3(),
		const glm::vec3& rotation = glm::vec3(),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 positionMatrix = glm::translate(position);
		glm::mat4 rotationXMatrix = glm::rotate(glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotationYMatrix = glm::rotate(glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZMatrix = glm::rotate(glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 scaleMatrix = glm::scale(scale);

		glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

		// Matrix multiplication is applied in reverse order
		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	inline glm::vec3& GetPosition() { return position; }
	inline glm::vec3& GetRotation() { return rotation; }
	inline glm::vec3& GetScale() { return scale; }

	inline void SetPosition(const glm::vec3& position) { this->position = position; }
	inline void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; }
	inline void SetScale(const glm::vec3& scale) { this->scale = scale; }

protected:
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};

