#include <iostream>
#include <limits>
#include <algorithm>
#include <GL/glew.h>
#include <SDL2/sdl.h>
#include <ctime>

#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Transform.h"
#include "Rendering/Camera.h"
#include "Application.h"
#include "GameEventHandler.h"
#include "GameRenderContext.h"
#include "InteractionWorld.h"
#include "Rendering/TextRenderer.h"
#include "Rendering/Text.h"
#include "Timing.h"

#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"
#include "GameComponentSystem/RotControlComponent.h"
#include "GameComponentSystem/RenderableMeshComponentSystem.h"
#include "GameComponentSystem/MotionComponentSystem.h"
#include "GameComponentSystem/CameraComponentSystem.h"


// Window size
#define WIDTH 640
#define HEIGHT 640

/**
 * @brief System which updates the position of the entity according to the state of the motion
 * component. Uses a motion integrator to determine the new position every update.
 */
class SpinningSystem : public BaseECSSystem
{
public:
	SpinningSystem() : BaseECSSystem()
	{
		AddComponentType(TransformComponent::ID);
	}

	virtual void UpdateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transform = (TransformComponent*)components[0];

		// Update position of the entity...
		glm::vec3 newRotation = transform->transform.GetRotation();
		newRotation.x += 30.0f * deltaTime;
		newRotation.y += 30.0f * deltaTime;
		newRotation.z += 30.0f * deltaTime;
		transform->transform.SetRotation(newRotation);
	}

private:
};

// TODO refactor main
int main(int argc, char** argv)
{
	Application* application = Application::Create();
	Window window(*application, WIDTH, HEIGHT, "GLEngine");
	RenderDevice device(window);
	Sampler sampler(device);

	Shader shader(device, "./Assets/Shaders/BasicShader.glsl");
	Shader shaderText(device, "./Assets/Shaders/TextShader.glsl");

	// Create a camera used for rendering
	Camera camera(70.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	RenderDevice::DrawParameters drawParameters;
	drawParameters.primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
	drawParameters.faceCulling = RenderDevice::FACE_CULL_BACK;
	drawParameters.depthFunc = RenderDevice::DRAW_FUNC_LESS;
	drawParameters.shouldWriteDepth = true;

	RenderTarget target(device);
	GameRenderContext gameRenderContext(device, target, drawParameters, shader, sampler, camera);

	std::vector<IndexedModel> models = LoadModels("./Assets/Models/Flag.obj");
	VertexArray vertexArray(device, models[0], RenderDevice::USAGE_STATIC_DRAW);

	// Load textures
	Texture textureFlag(device, "./Assets/Textures/Flag.png",RenderDevice::FORMAT_RGBA,false,false);

	// Create the text renderer and load fonts used
	TextRenderer textRenderer(WIDTH, HEIGHT, device, target, shaderText, sampler);

	Font* font = textRenderer.LoadFont("./Assets/Fonts/Arial.ttf", 64);

	std::vector<Text::Layer> style = {
		{ glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f / 32.f, .5f, Transform(glm::vec3(-5.f, -5.f, 0.f)) },
		{ glm::vec4(1.f, 0.f, 0.f, 1.f), 1.f / 3.f, .3f,  Transform() },
		{ glm::vec4(1.f, 0.f, 0.f, 1.f), 1.f / 32.f, .4f, Transform() },
		{ glm::vec4(1.f, 1.f, 1.f, 1.f), 1.f / 32.f, .5f, Transform() },
	};

	Text hwText(device, textRenderer, font, "Hello world!", Text::Anchor::CENTERED, style,
		Transform());

	// Create the ECS
	ECS ecs;
	// Systems which determine game logic
	ECSSystemList mainSystems;
	// Systems which determine rendering
	ECSSystemList renderingPipeline;
	// The interaction world, which determines how two entities should interact in the event of
	// collision
	InteractionWorld interactionWorld(ecs);
	// Add the interaction world to the ECS
	ecs.AddListener(&interactionWorld);

	// Create the event handler for responding to window events
	GameEventHandler eventHandler;

	AxisControl xControl;
	AxisControl yControl;
	MotionControl motionControl;
	eventHandler.AddMouseMotionControl(motionControl);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_W, yControl, 100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_S, yControl, -100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_A, xControl, -100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_D, xControl, 100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_UP, yControl, 100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_DOWN, yControl, -100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_LEFT, xControl, -100.f);
	eventHandler.AddKeyAxisControl(SDL_SCANCODE_RIGHT, xControl, 100.f);

	// Create components
	TransformComponent transformComponent;
	ColliderComponent colliderComponent;
	RotControlComponent rotControlComponent;
	MotionComponent motionComponent;
	CameraComponent cameraComponent;
	RenderableMeshComponent renderableMeshComponent;
	
	// Create the player entity...

	// Set the player position to the start of the maze
	transformComponent.transform.SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	// Set up the camera attached to the player
	// No offset; first person
	cameraComponent.camera = &camera;
	cameraComponent.offset = Transform(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	rotControlComponent.x = &xControl;
	rotControlComponent.y = &yControl;
	rotControlComponent.motion = &motionControl;

	// Set mesh and texture for the flag
	renderableMeshComponent.mesh = &vertexArray;
	renderableMeshComponent.texture = &textureFlag;

	// Finally, create the player!
	ecs.MakeEntity(transformComponent, renderableMeshComponent, rotControlComponent);

	// Create systems
	RotControlSystem rotControlSystem;
	MotionSystem motionSystem;
	CameraSystem cameraSystem;
	RenderableMeshSystem renderableMeshSystem(gameRenderContext);

	mainSystems.AddSystem(rotControlSystem);
	mainSystems.AddSystem(motionSystem);
	mainSystems.AddSystem(cameraSystem);
	renderingPipeline.AddSystem(renderableMeshSystem);

	// Time values used for calculating delta time
	float currentTime = Timing::GetTime(), previousTime = currentTime;

	// Game loop; keep updating until the window is closed
	while (application->IsRunning())
	{
		// Update time values
		currentTime = Timing::GetTime();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// Process application events; keypresses, mouse buttons/motion, window resizing, etc.
		application->ProcessMessages(deltaTime, eventHandler);

		// Update all game logic systems
		ecs.UpdateSystems(mainSystems, deltaTime);

		// Process any interactions (collisions) between entities
		interactionWorld.ProcessInteractions(deltaTime);

		// Clear the display for rendering the next frame
		gameRenderContext.Clear(0.6f, 0.8f, 1.0f, 1.0f, true);

		// Update the rendering pipeline
		ecs.UpdateSystems(renderingPipeline, deltaTime);

		gameRenderContext.Flush();
	
		style[1].color.a = abs(sin(Timing::GetTime() * 2));
		hwText.SetStyle(style);

		Transform textTransform;
		textTransform.SetPosition(glm::vec3(WIDTH / 2, HEIGHT / 2, 0));
		textTransform.SetRotation(glm::vec3(0.f, 0.f, sin(Timing::GetTime()) * 5.f));
		textTransform.SetScale(glm::vec3(-abs(sin(Timing::GetTime() * 3)) / 6 + 1.8));
		hwText.SetTransform(textTransform);

		hwText.SetText("Hello world " + std::to_string((int)Timing::GetTime()));

		textRenderer.RenderText(hwText);

		// Swap buffers
		window.Present();
	}

	delete application;

	return 0;
}