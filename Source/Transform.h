#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3(),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) : position(position),
		rotation(rotation), scale(scale) {}

	[[nodiscard]] glm::mat4 GetModel() const
	{
		const glm::mat4 positionMatrix = glm::translate(position);

		const glm::mat4 rotationXMatrix = glm::rotate(glm::radians(rotation.x), 
			glm::vec3(1.0, 0.0, 0.0));
		const glm::mat4 rotationYMatrix = glm::rotate(glm::radians(rotation.y), 
			glm::vec3(0.0, 1.0, 0.0));
		const glm::mat4 rotationZMatrix = glm::rotate(glm::radians(rotation.z),
			glm::vec3(0.0, 0.0, 1.0));

		const glm::mat4 scaleMatrix = glm::scale(scale);

		const glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

		// Matrix multiplication is applied in reverse order
		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	[[nodiscard]] glm::vec3& GetPosition() { return position; }
	[[nodiscard]] glm::vec3& GetRotation() { return rotation; }
	[[nodiscard]] glm::vec3& GetScale() { return scale; }

	[[nodiscard]] const glm::vec3& GetPosition() const { return position; }
	[[nodiscard]] const glm::vec3& GetRotation() const { return rotation; }
	[[nodiscard]] const glm::vec3& GetScale() const { return scale; }

	void SetPosition(const glm::vec3& position) { this->position = position; }
	void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; }
	void SetScale(const glm::vec3& scale) { this->scale = scale; }

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};

