#include <iostream>
#include <limits>
#include <algorithm>
#include <GL/glew.h>
#include <SDL2/sdl.h>
#include <ctime>

#include "Rendering/Display.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Transform.h"
#include "Rendering/Camera.h"
#include "Application.h"
#include "Events/GameEventHandler.h"
#include "Rendering/GameRenderContext.h"
#include "InteractionWorld.h"
#include "Rendering/TextRenderer.h"

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
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create window
	Display display(WIDTH, HEIGHT, "GLEngine");
	Application application(&display);

	// Load flag mesh (used for the end of the level)
	Mesh meshFlag("./Assets/Models/Flag.obj");
	
	// Load shaders
	// shaderBasic is used for drawing 3D geometry
	Shader shaderBasic("./Assets/Shaders/BasicShader");
	// shaderText is used for drawing text
	Shader shaderText("./Assets/Shaders/TextShader");

	// Load textures
	Texture textureFlag("./Assets/Textures/Flag.png");

	// Create a camera used for rendering
	Camera camera(70.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f, glm::vec3(0.0f));

	// See GameRenderContext for more info...
	GameRenderContext gameRenderContext(shaderBasic, camera);

	// Create the text renderer and load fonts used
	TextRenderer textRenderer(WIDTH, HEIGHT, shaderText);
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
	renderableMeshComponent.mesh = &meshFlag;
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

	// Time values used for calculating delta time and framerate
	unsigned int frameCounter = 0, frames = 0, fps = 0;
	unsigned int currentTime = SDL_GetTicks(), previousTime = currentTime;

	// Game loop; keep updating until the window is closed
	while (!application.IsClosed())
	{
		// Update time values
		currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - previousTime) / 1000.0f;
		frameCounter += (currentTime - previousTime);
		frames++;
		previousTime = currentTime;

		// If one second has passed, get the number of frames since the previous second
		// This value can be used to determine the frames per second
		if (frameCounter >= 1000)
		{
			frameCounter = 0;
			fps = frames;
			frames = 0;
		}

		// Process application events; keypresses, mouse buttons/motion, window resizing, etc.
		application.ProcessEvents(eventHandler);

		// Update all game logic systems
		ecs.UpdateSystems(mainSystems, deltaTime);

		// Process any interactions (collisions) between entities
		interactionWorld.ProcessInteractions(deltaTime);

		// Clear the display for rendering the next frame
		display.Clear(0.6f, 0.8f, 1.0f, 1.0f);

		// Update the rendering pipeline
		ecs.UpdateSystems(renderingPipeline, deltaTime);

		// Draw shadow first
		textRenderer.RenderText(fontLarge, "Hello world!",
			WIDTH/2 + 2.5, HEIGHT/2 - 2.5, 1.0f, glm::vec3(0.25f, 0.25f, 0.25f), true);
			
		textRenderer.RenderText(fontLarge, "Hello world!",
			WIDTH/2, HEIGHT/2, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);

		// Swap buffers
		display.Update();
	}

	// Quit SDL...
	SDL_Quit();

	return 0;
}