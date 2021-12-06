#version 330 core

in vec2 textureCoordinates;
out vec4 color;

uniform sampler2D text; // Mono-colored bitmap image of the glyph
uniform vec3 textColor; // Final color to use for text

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoordinates).r);
	color = vec4(textColor, 1.0) * sampled;
}
