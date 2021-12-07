#include "OpenGLRenderDevice.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

static bool AddShader(GLuint shaderProgram, const std::string& text, GLenum type, 
	std::vector<GLuint>* shaders);

static void AddAllAttributes(GLuint program, const std::string& vertexShaderText,
	unsigned int version);

static bool CheckShaderError(GLuint shader, int flag, bool isProgram, 
	const std::string& errorMessage);

static void AddShaderUniforms(GLuint shaderProgram, const std::string& shaderText,
	std::unordered_map<std::string, GLint>& uniformMap,
	std::unordered_map<std::string, GLint>& samplerMap);

bool OpenGLRenderDevice::isInitialized = false;

bool OpenGLRenderDevice::GlobalInit()
{
	// Already initialized
	if (isInitialized) return true;

	// OpenGL Version 3.3
	int major = 3;
	int minor = 3;

	isInitialized = true;
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
	{
		std::cerr << "Warning: Could not set core OpenGL profile" << std::endl;
		isInitialized = false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major) != 0)
	{
		std::cerr << "Error: Could not set major OpenGL version to " << major << ": "
			<< SDL_GetError() << std::endl;
		isInitialized = false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor) != 0)
	{
		std::cerr << "Error: Could not set minor OpenGL version to " << minor << ": "
			<< SDL_GetError() << std::endl;
		isInitialized = false;
	}

	return isInitialized;
}

OpenGLRenderDevice::OpenGLRenderDevice(Window& window) : 
	shaderVersion(""), 
	version(0),
	boundFBO(0),
	viewportFBO(0),
	boundVAO(0),
	boundShader(0),
	currentFaceCulling(FACE_CULL_NONE),
	currentDepthFunc(DRAW_FUNC_ALWAYS),
	currentSourceBlend(BLEND_FUNC_NONE),
	currentDestBlend(BLEND_FUNC_NONE),
	currentStencilFunc(DRAW_FUNC_ALWAYS),
	currentStencilTestMask((unsigned int)0xFFFFFFFF),
	currentStencilWriteMask((unsigned int)0xFFFFFFFF),
	currentStencilComparisonVal(0),
	currentStencilFail(STENCIL_KEEP),
	currentStencilPassButDepthFail(STENCIL_KEEP),
	currentStencilPass(STENCIL_KEEP),
	blendingEnabled(false),
	shouldWriteDepth(false),
	stencilTestEnabled(false),
	scissorTestEnabled(false)
{

}

OpenGLRenderDevice::~OpenGLRenderDevice()
{
}

unsigned int OpenGLRenderDevice::CreateRenderTarget(unsigned int texture, unsigned int width, 
	unsigned int height, FramebufferAttachment attachment, unsigned int attachmentNumber, 
	unsigned int mipLevel)
{
	unsigned int fbo;
	glGenBuffers(1, &fbo);
	SetFBO(fbo);

	GLenum attachmentTypeGL = attachment + attachmentNumber;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTypeGL, GL_TEXTURE_2D, texture, mipLevel);

	FBOData data;
	data.width = width;
	data.height = height;
	fboMap[fbo] = data;

	return fbo;
}

unsigned int OpenGLRenderDevice::ReleaseRenderTarget(unsigned int fbo)
{
	if (fbo == 0) return 0;

	std::unordered_map<unsigned int, FBOData>::iterator it = fboMap.find(fbo);

	if (it == fboMap.end())
	{
		return 0;
	}

	glDeleteFramebuffers(1, &fbo);
	fboMap.erase(it);
	return 0;
}

unsigned int OpenGLRenderDevice::CreateVertexArray(const float** vertexData, const unsigned int* vertexElementSizes, unsigned int numVertexComponents, unsigned int numInstanceComponents, unsigned int numVertices, const unsigned int* indices, unsigned int numIndices, BufferUsage usage)
{
	return 0;
}

void OpenGLRenderDevice::UpdateVertexArrayBuffer(unsigned int vao, unsigned int bufferIndex, const void* data, size_t dataSize)
{
}

unsigned int OpenGLRenderDevice::ReleaseVertexArray(unsigned int vao)
{
	return 0;
}

unsigned int OpenGLRenderDevice::CreateSampler(SamplerFilter minFilter, SamplerFilter magFilter, SamplerWrapMode wrapU, SamplerWrapMode wrapV, float anisotropy)
{
	return 0;
}

unsigned int OpenGLRenderDevice::ReleaseSampler(unsigned int sampler)
{
	return 0;
}

unsigned int OpenGLRenderDevice::CreateTexture2D(int width, int height, const void* data, PixelFormat dataFormat, PixelFormat internalFormat, bool generateMipmaps, bool compress)
{
	return 0;
}

unsigned int OpenGLRenderDevice::ReleaseTexture2D(unsigned int texture2D)
{
	return 0;
}

unsigned int OpenGLRenderDevice::CreateUniformBuffer(const void* data, size_t dataSize, BufferUsage usage)
{
	return 0;
}

void OpenGLRenderDevice::UpdateUniformBuffer(unsigned int buffer, const void* data, size_t dataSize)
{
}

unsigned int OpenGLRenderDevice::ReleaseUniformBuffer(unsigned int buffer)
{
	return 0;
}

unsigned int OpenGLRenderDevice::CreateShaderProgram(const std::string& shaderText)
{
	return 0;
}

void OpenGLRenderDevice::SetShaderUniformBuffer(unsigned int shader, const std::string& uniformBufferName, unsigned int buffer)
{
}

void OpenGLRenderDevice::SetShaderSampler(unsigned int shader, const std::string& samplerName, unsigned int texture, unsigned int sampler, unsigned int unit)
{
}

unsigned int OpenGLRenderDevice::ReleaseShaderProgram(unsigned int shader)
{
	return 0;
}

void OpenGLRenderDevice::Clear(unsigned int fbo, bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil, const float r, const float g, const float b, const float a, unsigned int stencil)
{
}

void OpenGLRenderDevice::Draw(unsigned int fbo, unsigned int shader, unsigned int vao, const DrawParameters& drawParameters, unsigned int numInstances, unsigned int numElements)
{
}

void OpenGLRenderDevice::SetFBO(unsigned int fbo)
{
}

void OpenGLRenderDevice::SetViewport(unsigned int fbo)
{
}

void OpenGLRenderDevice::SetVAO(unsigned int vao)
{
}

void OpenGLRenderDevice::SetShader(unsigned int shader)
{
}

void OpenGLRenderDevice::SetFaceCulling(FaceCulling faceCulling)
{
}

void OpenGLRenderDevice::SetDepthTest(bool shouldWrite, DrawFunc depthFunc)
{
}

void OpenGLRenderDevice::SetBlending(BlendFunc sourceBlend, BlendFunc destBlend)
{
}

void OpenGLRenderDevice::SetStencilTest(bool enable, DrawFunc stencilFunc, unsigned int stencilTestMask, unsigned int stencilWriteMask, unsigned int stencilComparisonVal, StencilOp stencilFail, StencilOp stencilPassButDepthFail, StencilOp stencilPass)
{
}

void OpenGLRenderDevice::SetStencilWriteMask(unsigned int mask)
{
}

void OpenGLRenderDevice::SetScissorTest(bool enable, unsigned int startX, unsigned int startY, unsigned int width, unsigned int height)
{
}
