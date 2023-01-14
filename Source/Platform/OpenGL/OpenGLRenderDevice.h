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

#include "Window.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <vector>
#include <string>
#include <unordered_map>

typedef SDL_GLContext DeviceContext;

/**
 * @brief Rendering interface/wrapper for OpenGL version 3.3 core. For details on OpenGL functions
 * (prefixed with gl), refer to the documentation which can be found here:
 *		- https://www.khronos.org/opengl/
 *		- https://docs.gl/
 */
class OpenGLRenderDevice
{
public:
	/**
	 * @brief Hints for what the user will be doing with the buffer and how frequently the user will
	 * be changing the buffer's data
	 *
	 * DRAW: The user will be writing data to the buffer, but the user will not read it.
	 * READ: The user will not be writing data, but the user will be reading it back.
	 * COPY: The user will be neither writing nor reading the data.
	 *
	 * STATIC: The user will set the data once.
	 * DYNAMIC: The user will set the data occasionally.
	 * STREAM: The user will be changing the data after every use, or almost every use.
	 */
	enum BufferUsage
	{
		USAGE_STATIC_DRAW = GL_STATIC_DRAW,
		USAGE_STREAM_DRAW = GL_STREAM_DRAW,
		USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,

		USAGE_STATIC_COPY = GL_STATIC_COPY,
		USAGE_STREAM_COPY = GL_STREAM_COPY,
		USAGE_DYNAMIC_COPY = GL_DYNAMIC_COPY,

		USAGE_STATIC_READ = GL_STATIC_READ,
		USAGE_STREAM_READ = GL_STREAM_READ,
		USAGE_DYNAMIC_READ = GL_DYNAMIC_READ,
	};

	enum SamplerFilter
	{
		FILTER_NEAREST = GL_NEAREST,
		FILTER_LINEAR = GL_LINEAR,
		FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum SamplerWrapMode
	{
		WRAP_CLAMP = GL_CLAMP_TO_EDGE,
		WRAP_REPEAT = GL_REPEAT,
		WRAP_CLAMP_MIRROR = GL_MIRROR_CLAMP_TO_EDGE,
		WRAP_REPEAT_MIRROR = GL_MIRRORED_REPEAT,
	};

	enum PixelFormat
	{
		FORMAT_R,
		FORMAT_RG,
		FORMAT_RGB,
		FORMAT_RGBA,
		FORMAT_DEPTH,
		FORMAT_DEPTH_AND_STENCIL,
	};

	enum PrimitiveType
	{
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,
		PRIMITIVE_POINTS = GL_POINTS,
		PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,
		PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,
		PRIMITIVE_LINES = GL_LINES,
		PRIMITIVE_LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		PRIMITIVE_LINES_ADJACENCY = GL_LINES_ADJACENCY,
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
		PRIMITIVE_TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		PRIMITIVE_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PRIMITIVE_PATCHES = GL_PATCHES,
	};

	enum FaceCulling
	{
		FACE_CULL_NONE,
		FACE_CULL_BACK = GL_BACK,
		FACE_CULL_FRONT = GL_FRONT,
		FACE_CULL_FRONT_AND_BACK = GL_FRONT_AND_BACK,
	};

	enum DrawFunc
	{
		DRAW_FUNC_NEVER = GL_NEVER,
		DRAW_FUNC_ALWAYS = GL_ALWAYS,
		DRAW_FUNC_LESS = GL_LESS,
		DRAW_FUNC_GREATER = GL_GREATER,
		DRAW_FUNC_LEQUAL = GL_LEQUAL,
		DRAW_FUNC_GEQUAL = GL_GEQUAL,
		DRAW_FUNC_EQUAL = GL_EQUAL,
		DRAW_FUNC_NOT_EQUAL = GL_NOTEQUAL,
	};

	enum FramebufferAttachment
	{
		ATTACHMENT_COLOR = GL_COLOR_ATTACHMENT0,
		ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
		ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
	};

	enum BlendFunc
	{
		BLEND_FUNC_NONE,
		BLEND_FUNC_ONE = GL_ONE,
		BLEND_FUNC_SRC_ALPHA = GL_SRC_ALPHA,
		BLEND_FUNC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		BLEND_FUNC_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		BLEND_FUNC_DST_ALPHA = GL_DST_ALPHA,
	};

	enum StencilOp
	{
		STENCIL_KEEP = GL_KEEP,
		STENCIL_ZERO = GL_ZERO,
		STENCIL_REPLACE = GL_REPLACE,
		STENCIL_INCR = GL_INCR,
		STENCIL_INCR_WRAP = GL_INCR_WRAP,
		STENCIL_DECR_WRAP = GL_DECR_WRAP,
		STENCIL_DECR = GL_DECR,
		STENCIL_INVERT = GL_INVERT,
	};

	struct DrawParameters
	{
		PrimitiveType primitiveType = PRIMITIVE_TRIANGLES;
		FaceCulling faceCulling = FACE_CULL_NONE;
		DrawFunc depthFunc = DRAW_FUNC_ALWAYS;
		bool shouldWriteDepth = true;
		bool useStencilTest = false;
		DrawFunc stencilFunc = DRAW_FUNC_ALWAYS;
		unsigned int stencilTestMask = 0;
		unsigned int stencilWriteMask = 0;
		unsigned int stencilComparisonVal = 0;
		StencilOp stencilFail = STENCIL_KEEP;
		StencilOp stencilPassButDepthFail = STENCIL_KEEP;
		StencilOp stencilPass = STENCIL_KEEP;
		bool useScissorTest = false;
		unsigned int scissorStartX = 0;
		unsigned int scissorStartY = 0;
		unsigned int scissorWidth = 0;
		unsigned int scissorHeight = 0;
		BlendFunc sourceBlend = BLEND_FUNC_NONE;
		BlendFunc destBlend = BLEND_FUNC_NONE;
	};

	/**
	 * @brief Sets OpenGL attributes/version if not already initialized.
	 * @return true if OpenGL is (or already was) successfully initialized.
	 */
	static bool GlobalInit();

	/**
	 * @param window Window to create the OpenGL context in.
	 * @todo Remove SDL2 dependency.
	 */
	OpenGLRenderDevice(Window& window);
	virtual ~OpenGLRenderDevice();

	/** 
	 * @brief Creates a framebuffer object (FBO) and attaches a texture image to the FBO.
	 * @param texture The ID of the texture object whose image is to be attached.
	 * @param width Framebuffer width.
	 * @param height Framebuffer height.
	 * @param attachment The logical attachment of the framebuffer (see OpenGL documentation for
	 *		glFramebufferTexture).
	 * @param attachmentNumber If using ATTACHMENT_COLOR, this is the color attachment number,
	 *		as there can be multiple color attachments.
	 * @param mipLevel The mipmap level of the texture image to be attached, which must be 0.
	 * @return ID of the created FBO.
	 */
	unsigned int CreateRenderTarget(unsigned int texture, unsigned int width, unsigned int height,
		FramebufferAttachment attachment, unsigned int attachmentNumber, unsigned int mipLevel);

	/**
	 * @brief Updates framebuffer size.
	 * @param fbo Target framebuffer object ID.
	 * @param width New width.
	 * @param height New height.
	 */
	void UpdateRenderTarget(unsigned int fbo, unsigned int width, unsigned int height);

	/**
	 * @brief Releases a framebuffer object (FBO).
	 * @param fbo ID of the FBO to release.
	 * @return FBO ID, 0, which is null.
	 */
	unsigned int ReleaseRenderTarget(unsigned int fbo);

	/**
	 * @brief Creates a vertex array object (VAO) which contains one vertex buffer object (VBO).
	 *		VBO data can be updated via UpdateVertexArrayBuffer.
	 * @param vertexData Two dimensional array of vertex data, where each inner array represents
	 *		per-vertex attribute data. If set to nullptr, no initial vertex data will be set.
	 * @param vertexElementSizes The size of each vertex element (each attribute size).
	 * @param numVertexComponents Number of per-vertex components/attributes.
	 * @param numInstanceComponents Number of per-instance components/attributes.
	 * @param numVertices Number of model vertices.
	 * @param indices Vertex indices, used to construct the primitive type.
	 * @param numIndices Number of vertex indices.
	 * @param usage Hint for how the vertex array buffer will be used.
	 * @return ID of the created VAO.
	 */
	unsigned int CreateVertexArray(const float** vertexData, const unsigned int* vertexElementSizes,
		unsigned int numVertexComponents, unsigned int numInstanceComponents,
		unsigned int numVertices, const unsigned int* indices, unsigned int numIndices, 
		BufferUsage usage);

	/**
	 * @brief 
	 * @param vao 
	 * @param bufferIndex 
	 * @param data 
	 * @param dataSize 
	 */
	void UpdateVertexArrayBuffer(unsigned int vao, unsigned int bufferIndex, const void* data,
		size_t dataSize);

	/**
	 * @brief Releases a vertex array object (VAO) and any associated vertex buffer objects (VBOs).
	 * @param vao ID of the VAO to release.
	 * @return VAO ID, 0, which is null.
	 */
	unsigned int ReleaseVertexArray(unsigned int vao);

	/**
	 * @brief Creates a sampler object.
	 * @param minFilter Minification filter. Determines how a texture is shrunk when sampled.
	 * @param magFilter Magnification filter. Determines how a texture is enlarged when sampled.
	 * @param wrapU Behavior when U texture coordinate is out of bounds.
	 * @param wrapV Behavior when V texture coordinate is out of bounds.
	 * @param anisotropy Maximum number of samples for anisotropic filtering.
	 * @return ID of the created sampler.
	 */
	unsigned int CreateSampler(SamplerFilter minFilter, SamplerFilter magFilter,
		SamplerWrapMode wrapU, SamplerWrapMode wrapV, float anisotropy);

	/**
	 * @brief Releases a sampler object.
	 * @param sampler ID of the sampler to release.
	 * @return Sampler ID, 0, which is null.
	 */
	unsigned int ReleaseSampler(unsigned int sampler);

	/**
	 * @brief Creates a 2D texture object.
	 * @param width Pixel width of the texture.
	 * @param height Pixel height of the texture.
	 * @param data Pointer to texture data.
	 * @param dataFormat Specifies the format of the pixel data.
	 * @param internalFormat Specifies the number of color components in the texture.
	 * @param generateMipmaps Whether mipmaps should be generated. This can help remove artifacts
	 *		when the image is scaled down (typically when viewed from far away in perspective).
	 * @param compress Whether OpenGL should attempt to compress the texture before storage.
	 * @param packAlignment Valid values are 1, 2, 4, or 8. Sets the alignment requirements for the
	 *		start of each pixel row in memory. Affects how pixel data is returned to client memory.
	 * @param unpackAlignment Valid values are 1, 2, 4, or 8. Sets the alignment requirements for
	 *		the start of each pixel row in memory. Affects how pixel data is read from client
	 *		memory.
	 * @return ID of the created 2D texture.
	 */
	unsigned int CreateTexture2D(int width, int height, const void* data, PixelFormat dataFormat, 
		PixelFormat internalFormat,	bool generateMipmaps, bool compress, int packAlignment, 
		int unpackAlignment);

	/**
	 * @brief Releases a 2D texture object.
	 * @param texture2D ID of the texture to release.
	 * @return Texture ID, 0, which is null.
	 */
	unsigned int ReleaseTexture2D(unsigned int texture2D);

	/**
	 * @brief Creates a uniform buffer object (UBO).
	 * @param data A pointer to data that will be copied into the data store for initialization, or
	 *		nullptr if no data is to be copied.
	 * @param dataSize The size in bytes of the buffer.
	 * @param usage Hint for how the uniform buffer will be used.
	 * @return ID of the created UBO.
	 */
	unsigned int CreateUniformBuffer(const void* data, size_t dataSize, BufferUsage usage);

	/**
	 * @brief Updates a uniform buffer's contents/data.
	 * @param buffer ID of the target UBO.
	 * @param data A pointer to data that will be copied into the data store.
	 * @param dataSize The size in bytes of the buffer.
	 */
	void UpdateUniformBuffer(unsigned int buffer, const void* data, size_t dataSize);

	/**
	 * @brief Releases a uniform buffer object (UBO).
	 * @param buffer ID of the UBO to release.
	 * @return UBO ID, 0, which is null.
	 */
	unsigned int ReleaseUniformBuffer(unsigned int buffer);


	unsigned int CreateShaderProgram(const std::string& shaderText);
	void SetShaderUniformBuffer(unsigned int shader, const std::string& uniformBufferName,
		unsigned int buffer);
	void SetShaderSampler(unsigned int shader, const std::string& samplerName, unsigned int texture, 
		unsigned int sampler, unsigned int unit);
	unsigned int ReleaseShaderProgram(unsigned int shader);

	/**
	 * @brief Sets an integer uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param value	Value to set the uniform to.
	 */
	void SetShaderInt(unsigned int shader, const std::string& name, int value);

	/**
	 * @brief Sets an integer array uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Array values to set the uniform to.
	 * @param count Number of values in the array.
	 */
	void SetShaderIntArray(unsigned int shader, const std::string& name, int* values, 
		uint32_t count);

	/**
	 * @brief Sets a float uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param value Value to set the uniform to.
	 */
	void SetShaderFloat(unsigned int shader, const std::string& name, float value);

	/**
	 * @brief Sets a float2 (a.k.a. vec2) uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Pointer to the first element in a two-element array.
	 */
	void SetShaderFloat2(unsigned int shader, const std::string& name, const float* values);

	/**
	 * @brief Sets a float3 (a.k.a. vec3) uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Pointer to the first element in a three-element array.
	 */
	void SetShaderFloat3(unsigned int shader, const std::string& name, const float* values);

	/**
	 * @brief Sets a float4 (a.k.a. vec4) uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Pointer to the first element in a four-element array.
	 */
	void SetShaderFloat4(unsigned int shader, const std::string& name, const float* values);

	/**
	 * @brief Sets a mat3 uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Pointer to the first element in a nine-element (3x3) array.
	 */
	void SetShaderMat3(unsigned int shader, const std::string& name, const float* values);

	/**
	 * @brief Sets a mat4 uniform for a shader.
	 * @param shader Shader ID.
	 * @param name Uniform variable name.
	 * @param values Pointer to the first element in a sixteen-element (4x4) array.
	 */
	void SetShaderMat4(unsigned int shader, const std::string& name, const float* values);

	// TODO there are many types of shader uniforms which do not have setters yet

	/**
	 * @brief Clears a framebuffer.
	 * @param fbo The target framebuffer object for clearing.
	 * @param shouldClearColor Whether or not the FBO's color buffer should be cleared.
	 * @param shouldClearDepth Whether or not the FBO's depth buffer should be cleared.
	 * @param shouldClearStencil Whether or not the FBO's stencil buffer should be cleared.
	 * @param r Red value of the color to fill the color buffer with (if it is cleared).
	 * @param g Green value of the color to fill the color buffer with (if it is cleared).
	 * @param b Blue value of the color to fill the color buffer with (if it is cleared).
	 * @param a Alpha value of the color to fill the color buffer with (if it is cleared).
	 * @param stencil Stencil mask ID. See glStencilMask for more details.
	 */
	void Clear(unsigned int fbo, bool shouldClearColor, bool shouldClearDepth, 
		bool shouldClearStencil, float r, float g, float b, float a, unsigned int stencil);

	/**
	 * @brief Draws to a framebuffer.
	 * @param fbo The target framebuffer object for drawing.
	 * @param shader ID of the shader to use.
	 * @param vao ID of the vertex array object to use.
	 * @param drawParameters See DrawParameters for all options.
	 * @param numInstances Number of times the model will be drawn, the "number of instances".
	 * @param numElements Number of vertices being drawn. Not to be confused with the number of
	 *		vertices in a model/vertex array; this count includes duplicate vertices which are
	 *		represented with the vertex array's indices.
	 */
	void Draw(unsigned int fbo, unsigned int shader, unsigned int vao, 
		const DrawParameters& drawParameters, unsigned int numInstances, unsigned int numElements);

	/** 
	 * @brief Setter for draw parameters.
	 * @see DrawParameters
	 */
	void SetDrawParameters(const DrawParameters& drawParameters);

private:
	// Disallow copy and assign
	OpenGLRenderDevice(const OpenGLRenderDevice& other) = delete;
	void operator=(const OpenGLRenderDevice& other) = delete;

	struct VertexArray
	{
		unsigned int* buffers;
		size_t* bufferSizes;
		unsigned int numBuffers;
		unsigned int numElements;
		unsigned int instanceComponentsStartIndex;
		BufferUsage usage;
	};

	struct ShaderProgram
	{
		std::vector<unsigned int> shaders;
		std::unordered_map<std::string, int> uniformMap;
		std::unordered_map<std::string, int> samplerMap;
	};

	struct FBOData
	{
		unsigned int width;
		unsigned int height;
	};

	void SetFBO(unsigned int fbo);
	void SetViewport(unsigned int fbo);
	void SetVAO(unsigned int vao);
	void SetShader(unsigned int shader);
	void SetFaceCulling(FaceCulling faceCulling);
	void SetDepthTest(bool shouldWrite, DrawFunc depthFunc);
	void SetBlending(BlendFunc sourceBlend, BlendFunc destinationBlend);
	void SetStencilTest(bool enable, DrawFunc stencilFunc, unsigned int stencilTestMask,
		unsigned int stencilWriteMask, unsigned int stencilComparisonVal, StencilOp stencilFail, 
		StencilOp stencilPassButDepthFail, StencilOp stencilPass);
	void SetStencilWriteMask(unsigned int mask);
	void SetScissorTest(bool enable, unsigned int startX = 0, unsigned int startY = 0,
		unsigned int width = 0, unsigned int height = 0);

	unsigned int GetVersion();
	std::string GetShaderVersion();

	static bool isInitialized;
	DeviceContext context;
	std::string shaderVersion;
	unsigned int version;
	std::unordered_map<unsigned int, VertexArray> vaoMap;
	std::unordered_map<unsigned int, FBOData> fboMap;
	std::unordered_map<unsigned int, ShaderProgram> shaderProgramMap;

	unsigned int boundFBO;
	unsigned int viewportFBO;
	unsigned int viewportWidth;
	unsigned int viewportHeight;
	unsigned int boundVAO;
	unsigned int boundShader;
	FaceCulling currentFaceCulling;
	DrawFunc currentDepthFunc;
	BlendFunc currentSourceBlend;
	BlendFunc currentDestBlend;
	DrawFunc currentStencilFunc;
	unsigned int currentStencilTestMask;
	unsigned int currentStencilWriteMask;
	int currentStencilComparisonVal;
	StencilOp currentStencilFail;
	StencilOp currentStencilPassButDepthFail;
	StencilOp currentStencilPass;
	bool blendingEnabled;
	bool shouldWriteDepth;
	bool stencilTestEnabled;
	bool scissorTestEnabled;
	int currentPackAlignment;
	int currentUnpackAlignment;
};