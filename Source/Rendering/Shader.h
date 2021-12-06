#pragma once

#include "Transform.h"
#include "Camera.h"

#include <string>
#include <GL/glew.h>

class Shader
{
public:
	/**
	 * Loads a shader from a file.
	 *
	 * @param fileName File path to the shader without extension. The constructor will automatically
	 *		assume that a *.vert file exists for the vertex shader and that a *.frag file exists
	 *		for the fragment shader.
	 */
	Shader(const std::string& fileName);

	/** @brief Instructs the GPU to bind/use the shader. */
	void Bind();

	/** @brief Update the transformation (model view projection) matrix */
	void Update(const glm::mat4& matrix);

	/**
	 * @brief Update the transformation (model view projection) matrix.
	 * 
	 * @param transform The transform, which contains the model matrix to use.
	 * @param camera The camera, which contains the view projection matrix to use.
	 */
	void Update(const Transform& transform, const Camera& camera);

	inline GLuint GetProgram() { return program; }

	virtual ~Shader();

protected:
private:
	static const unsigned int NUM_SHADERS = 2;

	// Disallow copy and assign
	Shader(const Shader& other) = delete;
	void operator=(const Shader& other) = delete;

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS,
	};

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];
};

