/*  
 * Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt
 *  
 * This file incorporates work covered by the following copyright and permission notice:  
 *  
 *     Copyright (c) 2018 Benny Bobaganoosh
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *     
 *     The above copyright notice and this permission notice shall be included in all
 *     copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *     SOFTWARE.
 */

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

