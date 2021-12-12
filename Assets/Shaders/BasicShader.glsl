#version 330 core

#if defined(VERTEX_SHADER_BUILD)

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in vec3 normal;
layout (location = 4) in mat4 transform;

out vec2 textureCoordinate0;
out vec3 normal0;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	textureCoordinate0 = textureCoordinate;
	normal0 = (transform * vec4(normal, 0.0)).xyz;
}

#elif defined(FRAGMENT_SHADER_BUILD)

in vec2 textureCoordinate0;
in vec3 normal0;

out vec4 color;

uniform sampler2D diffuse;

void main()
{
	color = texture(diffuse, textureCoordinate0);
	//color.xyz *= clamp(dot(-vec3(0, 0, 1), normal0), 0.4, 1.0);
	//color = vec4(1, 0, 0, 1);
}

#endif