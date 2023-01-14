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