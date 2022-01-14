#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/rotate_vector.hpp>

class Camera
{
public:
	/**
	 * Creates a camera for use in rendering.
	 * 
	 * @param fov Field of view; how wide the viewing angle is.
	 * @param aspect The aspect ratio of the camera.
	 * @param zNear The near Z clipping plane. Geometry behind this plane will not be rendered. Keep
	 *		as high as possible to avoid precision issues.
	 * @param zFar The far Z clipping plane. Geometry past this plane will not be rendered. Keep as
	 *		low as possible to avoid precision issues.
	 * @param position The initial position of the camera.
	 * @param rotation The initial rotation of the camera. Defaults to zero.
	 */
	Camera(float fov, float aspect, float zNear, float zFar, const glm::vec3& position, 
		const glm::vec3& rotation = glm::vec3(0.0f)) : fov(fov), aspect(aspect), zNear(zNear),
		zFar(zFar)
	{
		this->position = position;
		this->rotation = rotation;

		perspective = glm::perspective(fov, aspect, zNear, zFar);
		forward = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);
	}

	/**
	 * This transformation matrix should be applied to all geometry to achieve the "illusion" of
	 * a camera. All geometry in the world is rendered relative to the camera.
	 * 
	 * @return The view projection matrix.
	 */
	inline glm::mat4 GetViewProjection() const
	{
		glm::vec3 forward = this->forward;

		forward = glm::rotate(forward, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		forward = glm::rotate(forward, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		forward = glm::rotate(forward, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 up = this->up;

		up = glm::rotate(up, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		up = glm::rotate(up, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		up = glm::rotate(up, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		return perspective * glm::lookAt(position, position + forward, up);
	}

	inline void SetPosition(const glm::vec3& position) { this->position = position; }
	inline glm::vec3& GetPosition() { return position; }
	
	inline void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; }
	inline glm::vec3& GetRotation() { return rotation; }

	inline void SetFOV(float fov)
	{
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		this->fov = fov;
	}

	inline void SetAspect(float aspect)
	{
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		this->aspect = aspect;
	}

	inline void SetClippingPlane(float zNear, float zFar)
	{
		perspective = glm::perspective(fov, aspect, zNear, zFar);
		this->zNear = zNear;
		this->zFar = zFar;
	}

private:
	glm::mat4 perspective;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 forward;
	glm::vec3 up;

	float fov;
	float aspect;
	float zNear;
	float zFar;
};

