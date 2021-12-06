#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 textureCoordinates>
out vec2 textureCoordinates;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(vertex.xy, 0.0, 1.0);
	textureCoordinates = vertex.zw;
}