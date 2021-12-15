#version 330 core

#if defined(VERTEX_SHADER_BUILD)

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoordinate;

out vec2 textureCoordinate0;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 0.0, 1.0);
	textureCoordinate0 = textureCoordinate;
}

#elif defined(FRAGMENT_SHADER_BUILD)

in vec2 textureCoordinate0;
out vec4 color;

uniform sampler2D text; // Mono-colored bitmap image of the glyph
uniform vec3 textColor; // Final color to use for text

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoordinate0).r);
	color = vec4(textColor, 1.0) * sampled;
}

#endif