#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

#include <map>
#include <vector>

/**
 * @brief The game render context bridges the gap between the high-level rendering commands,
 * such as drawing a particular mesh, and the low-level rendering commands which are used for
 * rendering.
 */
class GameRenderContext
{
public:
	/**
	 * Creates a game render context.
	 * 
	 * @param shader The shader to use for rendering.
	 * @param camera The camera to use for rendering.
	 */
	GameRenderContext(Shader& shader, Camera& camera) : shader(shader), camera(camera) {}

	/**
	 * Renders a mesh. This will use the shader and camera specified in the constructor.
	 * 
	 * @param mesh The mesh to draw.
	 * @param texture The texture to use on the mesh.
	 * @param transform The transformation to apply to the mesh.
	 */
	inline void RenderMesh(Mesh& mesh, Texture& texture, const Transform& transform)
	{
		shader.Bind();
		texture.Bind(0);
		shader.Update(transform, camera);
		mesh.Draw();
	}

private:
	Shader& shader;
	Camera& camera;
};