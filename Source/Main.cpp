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
#include "Events/GameEventHandler.h"
#include "GameRenderContext.h"
#include "InteractionWorld.h"
#include "Rendering/TextRenderer.h"
#include "Timing.h"

#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"
#include "GameComponentSystem/MovementControlComponentSystem.h"
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
	Sampler sampler(device, RenderDevice::FILTER_LINEAR_MIPMAP_LINEAR, RenderDevice::FILTER_NEAREST);

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
	Font fontSmall = textRenderer.LoadFont("./Assets/Fonts/ArialBold.ttf", 16);
	Font fontLarge = textRenderer.LoadFont("./Assets/Fonts/ArialBold.ttf", 48);

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

	// Create components
	TransformComponent transformComponent;
	ColliderComponent colliderComponent;
	MovementControlComponent movementControlComponent;
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

	// Set mesh and texture for the flag
	renderableMeshComponent.mesh = &vertexArray;
	renderableMeshComponent.texture = &textureFlag;

	// Finally, create the player!
	ecs.MakeEntity(transformComponent, renderableMeshComponent, motionComponent);

	// Create systems
	MovementControlSystem movementControlSystem;
	MotionSystem motionSystem;
	CameraSystem cameraSystem;
	SpinningSystem spinningSystem;
	RenderableMeshSystem renderableMeshSystem(gameRenderContext);

	mainSystems.AddSystem(movementControlSystem);
	mainSystems.AddSystem(motionSystem);
	mainSystems.AddSystem(cameraSystem);
	mainSystems.AddSystem(spinningSystem);
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

		// Draw shadow first
		textRenderer.RenderText(fontLarge, "Hello world!",
			WIDTH/2 + 2.5, HEIGHT/2 - 2.5, 1.0f, glm::vec3(0.25f, 0.25f, 0.25f), true);
			
		textRenderer.RenderText(fontLarge, "Hello world!",
			WIDTH/2, HEIGHT/2, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);


		// Swap buffers
		window.Present();
	}

	delete application;

	return 0;
}