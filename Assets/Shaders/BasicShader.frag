#version 330 core

in vec2 textureCoordinate0;
in vec3 normal0;

out vec4 color;

uniform sampler2D diffuse;

void main()
{
	color = texture(diffuse, textureCoordinate0);
	//color.xyz *= clamp(dot(-vec3(0, 0, 1), normal0), 0.4, 1.0);
}