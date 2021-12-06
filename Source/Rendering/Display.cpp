#include "Display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
	this->width = width;
	this->height = height;

	// Use 32-bit color size
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Set OpenGL version to 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	// Use double buffering
	// This allows us to swap buffers once we finish drawing a frame
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create our window
	// Position it to the center of the screen and set flags
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Create an OpenGL context for our window, and make it current
	glContext = SDL_GL_CreateContext(window);

	// Initialize GLEW
	GLenum status = glewInit();

	// If GLEW failed to initialize
	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}

	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Enable Z buffering (a.k.a. depth buffering)
	// If a pixel is behind an already drawn pixel, then it will not be drawn
	glEnable(GL_DEPTH_TEST);

	// Cull back-facing polygons
	// We don't need to see the back side of a polygon
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable blending
	// This allows us to use transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Update()
{
	// Swap buffers
	SDL_GL_SwapWindow(window);
}

void Display::Resize(int width, int height)
{
	this->width = width;
	this->height = height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

Display::~Display()
{
	// Do this in reverse order to avoid allocation bugs
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
}
