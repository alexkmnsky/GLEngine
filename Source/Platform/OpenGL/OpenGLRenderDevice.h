#pragma once

#include "Window.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <vector>
#include <string>
#include <unordered_map>

typedef SDL_GLContext DeviceContext;

class OpenGLRenderDevice
{
public:
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
		PRIMITIVE_TRAINGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
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
		STENICL_INCR = GL_INCR,
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
	 * Sets OpenGL attributes/version if not already initialized.
	 * 
	 * @return true if OpenGL is (or already was) successfully initialized.
	 */
	static bool GlobalInit();

	OpenGLRenderDevice(Window& window);
	virtual ~OpenGLRenderDevice();

	unsigned int CreateRenderTarget(unsigned int texture, unsigned int width, unsigned int height,
		FramebufferAttachment attachment, unsigned int attachmentNumber, unsigned int mipLevel);
	unsigned int ReleaseRenderTarget(unsigned int fbo);

	unsigned int CreateVertexArray(const float** vertexData, const unsigned int* vertexElementSizes,
		unsigned int numVertexComponents, unsigned int numInstanceComponents,
		unsigned int numVertices, const unsigned int* indices, unsigned int numIndices, 
		BufferUsage usage);
	void UpdateVertexArrayBuffer(unsigned int vao, unsigned int bufferIndex, const void* data,
		size_t dataSize);
	unsigned int ReleaseVertexArray(unsigned int vao);

	unsigned int CreateSampler(SamplerFilter minFilter, SamplerFilter magFilter,
		SamplerWrapMode wrapU, SamplerWrapMode wrapV, float anisotropy);
	unsigned int ReleaseSampler(unsigned int sampler);

	unsigned int CreateTexture2D(int width, int height, const void* data, PixelFormat dataFormat, 
		PixelFormat internalFormat,	bool generateMipmaps, bool compress);
	unsigned int ReleaseTexture2D(unsigned int texture2D);

	unsigned int CreateUniformBuffer(const void* data, size_t dataSize, BufferUsage usage);
	void UpdateUniformBuffer(unsigned int buffer, const void* data, size_t dataSize);
	unsigned int ReleaseUniformBuffer(unsigned int buffer);

	unsigned int CreateShaderProgram(const std::string& shaderText);
	void SetShaderUniformBuffer(unsigned int shader, const std::string& uniformBufferName,
		unsigned int buffer);
	void SetShaderSampler(unsigned int shader, const std::string& samplerName, unsigned int texture, 
		unsigned int sampler, unsigned int unit);
	unsigned int ReleaseShaderProgram(unsigned int shader);

	void Clear(unsigned int fbo, bool shouldClearColor, bool shouldClearDepth, 
		bool shouldClearStencil, const float r, const float g, const float b, const float a,
		unsigned int stencil);

	void Draw(unsigned int fbo, unsigned int shader, unsigned int vao, 
		const DrawParameters& drawParameters, unsigned int numInstances, unsigned int numElements);

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
		int width;
		int height;
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
};