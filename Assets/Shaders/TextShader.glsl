#version 330 core

#if defined(VERTEX_SHADER_BUILD)

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in vec4 textColor;
layout (location = 3) in float smoothing;
layout (location = 4) in float offset;
layout (location = 5) in mat4 transform;

out vec2 textureCoordinate0;
out vec4 textColor0;
out float smoothing0;
out float offset0;

void main()
{
	gl_Position = transform * vec4(position, 0.0, 1.0);
	textureCoordinate0 = textureCoordinate;
	textColor0 = textColor;
	smoothing0 = smoothing;
	offset0 = offset;
}

#elif defined(FRAGMENT_SHADER_BUILD)

in vec2 textureCoordinate0;

// Final color to use for text
in vec4 textColor0; 

// Controls how smooth the text edges are (0 = none, 1 = full)
in float smoothing0; 

// Controls what the distance threshold is (0.5 for intended shape, < 0.5 for shape with increased
// border, > 0.5 for shape with decreased border
in float offset0;

out vec4 color;

// Mono-colored bitmap image of the signed-distance field for the glyph
uniform sampler2D texture0; 

void main()
{
	float sampledDistance = texture(texture0, textureCoordinate0).r;
	float alpha = smoothstep(offset0 - smoothing0, offset0 + smoothing0, sampledDistance);

	color = vec4(textColor0.xyz, textColor0.w * alpha);
}

#endif