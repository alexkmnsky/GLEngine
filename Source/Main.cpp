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
#include "CollisionResponseInteraction.h"

#include "Maze.h"

// Window size
#define WIDTH 1280
#define HEIGHT 720

// Grid size of the maze
#define MAZE_SIZE 19

// Forward declaration of maze world for the component below...
class MazeWorld;

/**
 * @brief Component used for marking the end of the level. Contains a pointer to the world so that
 * the LevelEndInteraction can dispatch a OnLevelEnd event to the world.
 */
struct LevelEndComponent : public ECSComponent<LevelEndComponent>
{
	MazeWorld* world;
};

/** @brief Components used in the MazeWorld */
struct MazeWorldComponents
{
	TransformComponent& transform;
	ColliderComponent& collider;
	RenderableMeshComponent& renderableMesh;
	LevelEndComponent& levelEnd;
};

class MazeWorld
{
public: 
	MazeWorld(ECS& ecs, Maze& maze, int width, int height, MazeWorldComponents& components, 
		Mesh& meshMaze,	Mesh& meshFlag, Texture& textureMaze, Texture& textureFlag) : ecs(ecs),
		components(components)
	{
		// Create the visible mesh of the maze
		components.transform.transform.SetPosition(glm::vec3(0.0f));
		components.renderableMesh.mesh = &meshMaze;
		components.renderableMesh.texture = &textureMaze;
		ecs.MakeEntity(components.transform, components.renderableMesh);

		// Create the flag entity which marks the end of the level...
		// Set the position of the flag correctly within the maze
		components.transform.transform.SetPosition(glm::vec3(
			(float)width * Maze::cellWidth - 3.0f,
			1.0f,
			-(float)height * Maze::cellWidth + 3.0f));
		// Rotate by 45 degrees
		components.transform.transform.SetRotation(glm::vec3(0.0f, 45.0f, 0.0f));
		// Set mesh and texture for the flag
		components.renderableMesh.mesh = &meshFlag;
		components.renderableMesh.texture = &textureFlag;
		components.levelEnd.world = this;
		// Set collider bounds and overlap
		components.collider.aabb = AABB(glm::vec3(-0.25f, -1.0f, -0.25f), glm::vec3(0.5f, 1.0f, 0.25f));
		components.collider.shouldOverlap = true;
		flag = ecs.MakeEntity(components.transform, components.collider, components.renderableMesh,
			components.levelEnd);

		// Create the bounds of the maze
		// This prevents the player from going through walls
		std::vector<Transform> aabbMazeTransforms;
		AABB aabbMaze = maze.GenerateAABBs(aabbMazeTransforms);
		components.collider.aabb = aabbMaze;
		components.collider.shouldOverlap = false;
		for (Transform& transform : aabbMazeTransforms)
		{
			components.transform.transform = transform;
			bounds.push_back(ecs.MakeEntity(components.transform, components.collider));
		}
	}

	/** @brief Should be called when the player reaches the end of the level. */
	inline void OnLevelEnd()
	{
		isLevelEnded = true;

		ecs.RemoveEntity(flag);
	}

	/** @brief Checks if the player has reached the end of the level. */
	inline bool IsLevelEnded() { return isLevelEnded; }

private:
	ECS& ecs;
	MazeWorldComponents& components;
	bool isLevelEnded = false;
	EntityHandle flag;
	std::vector<EntityHandle> bounds;
};

/** @brief Interaction used to detect when the player reaches the end of the level. */
class LevelEndInteraction : public Interaction
{
public:
	LevelEndInteraction() : Interaction(), mazeWorld(nullptr)
	{
		AddInteractorComponentType(TransformComponent::ID);
		AddInteractorComponentType(ColliderComponent::ID);
		AddInteracteeComponentType(TransformComponent::ID);
		AddInteracteeComponentType(ColliderComponent::ID);
		AddInteracteeComponentType(LevelEndComponent::ID);
	}

	virtual void Interact(float deltaTime, EntityHandle interactor, EntityHandle interactee,
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents)
	{
		LevelEndComponent* levelEndComponent = ((LevelEndComponent*)interacteeComponents[2]);
		isLevelEndReached = true;
		mazeWorld = levelEndComponent->world;
	}

	virtual void Finalize(float deltaTime)
	{
		if (isLevelEndReached)
		{
			// Dispatch a level end event to the world
			mazeWorld->OnLevelEnd();

			isLevelEndReached = false;
		}
	}

private:
	bool isLevelEndReached = false;
	MazeWorld* mazeWorld;
};

// TODO refactor main
int main(int argc, char** argv)
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create window
	Display display(WIDTH, HEIGHT, "Maze Game");
	Application application(&display);

	// Load flag mesh (used for the end of the level)
	Mesh meshFlag("./Assets/Models/Flag.obj");
	
	// Load shaders
	// shaderBasic is used for drawing 3D geometry
	Shader shaderBasic("./Assets/Shaders/BasicShader");
	// shaderText is used for drawing text
	Shader shaderText("./Assets/Shaders/TextShader");

	// Load textures used for the maze and flag
	Texture textureMaze("./Assets/Textures/FloorWall.png");
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

	// Player movement controls
	InputControl right;
	InputControl forward;
	MotionControl horizontal;
	MotionControl vertical;

	// Misc controls
	InputControl lockMouseControl;

	// Register controls
	eventHandler.AddKeyControl(SDL_Scancode::SDL_SCANCODE_W, forward, 1.0f);
	eventHandler.AddKeyControl(SDL_Scancode::SDL_SCANCODE_S, forward, -1.0f);
	eventHandler.AddKeyControl(SDL_Scancode::SDL_SCANCODE_A, right, -1.0f);
	eventHandler.AddKeyControl(SDL_Scancode::SDL_SCANCODE_D, right, 1.0f);
	eventHandler.AddMotionControl(horizontal, MotionControl::Axis::X, 1.0f);
	eventHandler.AddMotionControl(vertical, MotionControl::Axis::Y, 1.0f);
	eventHandler.AddMouseControl(3, lockMouseControl, 1.0f); // Right click

	// Create components
	TransformComponent transformComponent;
	ColliderComponent colliderComponent;
	MovementControlComponent movementControlComponent;
	MotionComponent motionComponent;
	CameraComponent cameraComponent;
	LevelEndComponent levelEndComponent;
	RenderableMeshComponent renderableMeshComponent;

	// Create the player entity...

	// Set the player position to the start of the maze
	transformComponent.transform.SetPosition(glm::vec3(3.0f, 1.0f, -3.0f));
	// Define the player bounding box
	colliderComponent.aabb = AABB(glm::vec3(-0.5f, -1.0f, -0.5f), glm::vec3(0.5f, 1.0f, 0.5f));
	// Add player input controls for W A S D movement
	movementControlComponent.inputControls.push_back(
		MovementInputControl(glm::vec3(1.0f, 0.0f, 0.0f) * 75.0f, &right));
	movementControlComponent.inputControls.push_back(
		MovementInputControl(glm::vec3(0.0f, 0.0f, -1.0f) * 75.0f, &forward));
	// Add player motion controls for turning the camera
	movementControlComponent.motionControls.push_back(
		MovementMotionControl(glm::vec3(0.0f, -1.0f, 0.0f) * 0.1f, &horizontal));
	movementControlComponent.motionControls.push_back(
		MovementMotionControl(glm::vec3(-1.0f, 0.0f, 0.0f) * 0.1f, &vertical));
	// Set up the camera attached to the player
	// No offset; first person
	cameraComponent.camera = &camera;
	cameraComponent.offset = Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Finally, create the player!
	ecs.MakeEntity(transformComponent, colliderComponent, movementControlComponent,
		motionComponent, cameraComponent);

	// Get the components needed for the maze world
	MazeWorldComponents mazeWorldComponents = { transformComponent, colliderComponent,
		renderableMeshComponent, levelEndComponent };

	// Initialize the maze
	Maze maze(MAZE_SIZE, MAZE_SIZE, (unsigned int)std::time(nullptr));
	IndexedModel modelMaze = maze.GenerateModel();
	Mesh meshMaze(modelMaze);

	// Create the maze world
	MazeWorld mazeWorld(ecs, maze, MAZE_SIZE, MAZE_SIZE, mazeWorldComponents, meshMaze, meshFlag,
		textureMaze, textureFlag);

	// Create systems
	MovementControlSystem movementControlSystem;
	MotionSystem motionSystem;
	CameraSystem cameraSystem;
	RenderableMeshSystem renderableMeshSystem(gameRenderContext);

	mainSystems.AddSystem(movementControlSystem);
	mainSystems.AddSystem(motionSystem);
	mainSystems.AddSystem(cameraSystem);
	renderingPipeline.AddSystem(renderableMeshSystem);

	// Create interactions
	CollisionResponseInteraction collisionResponseInteraction;
	LevelEndInteraction levelEndInteraction;
	interactionWorld.AddInteraction(&collisionResponseInteraction);
	interactionWorld.AddInteraction(&levelEndInteraction);

	// Set dialogue for the start and end of the game...

	const std::vector<std::string> dialogueStart = {
		"Welcome to my maze game!",
		"Try using W, A, S, D to move around.",
		"You've been placed in a randomly generated maze.",
		"I challenge you to find the flag!" };

	const std::vector<std::string> dialogueEnd = {
		"You reached the end!",
		"Thank you for playing my maze game!"};
	
	// Time values used for calculating delta time and framerate
	unsigned int currentTime = SDL_GetTicks(), previousTime = currentTime;
	unsigned int frameCounter = 0, frames = 0, fps = 0;

	// Values used for determining if the mouse lock should be toggled/if the mouse is locked
	float previousInputLockControlAmount = lockMouseControl.GetAmount();
	bool isMouseLocked = false;

	// Values used for timing the dialogue
	unsigned int levelStartTime = currentTime;
	unsigned int levelEndTime = 0;
	unsigned int dialogueInterval = 5000;
	std::string currentDialogue = "";

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

		// Mouse lock toggle
		// If a change in the input was detected
		if (lockMouseControl.GetAmount() != previousInputLockControlAmount)
		{
			// If the mouse button is being pressed
			if (lockMouseControl.GetAmount())
				// Toggle the mouse lock
				isMouseLocked = application.ToggleMouseLock();

			// Set previous value for detecting if there is a change in input
			previousInputLockControlAmount = lockMouseControl.GetAmount();
		}

		// Update all game logic systems
		ecs.UpdateSystems(mainSystems, deltaTime);

		// Process any interactions (collisions) between entities
		interactionWorld.ProcessInteractions(deltaTime);

		// If the end of the level was reached, start playing the end dialogue
		if (mazeWorld.IsLevelEnded())
		{
			unsigned int timeSinceLevelEnd = currentTime - levelEndTime;
			unsigned int dialogueIndex = timeSinceLevelEnd / dialogueInterval;

			// Go through the dialogue list, at intervals of dialogueInterval ms
			if (dialogueIndex < dialogueEnd.size())
			{
				currentDialogue = dialogueEnd[dialogueIndex];
			}
			else
			{
				// Show how long the player took to find the flag
				currentDialogue = "Time spent: " + 
					std::to_string((levelEndTime - levelStartTime)/1000) + " seconds!";
			}
		}
		else
		{
			unsigned int timeSinceLevelStart = currentTime - levelStartTime;
			unsigned int dialogueIndex = timeSinceLevelStart / dialogueInterval;

			// Go through the dialogue list, at intervals of dialogueInterval ms
			if (dialogueIndex < dialogueStart.size())
			{
				currentDialogue = dialogueStart[dialogueIndex];
			}
			// If the end of the dialogue was reached, hide the dialogue
			else
			{
				currentDialogue = "";
			}

			levelEndTime = currentTime;
		}

		// Clear the display for rendering the next frame
		display.Clear(0.6f, 0.8f, 1.0f, 1.0f);

		// Update the rendering pipeline
		ecs.UpdateSystems(renderingPipeline, deltaTime);

		// Render the info text in the top left
		textRenderer.RenderText(fontSmall, (isMouseLocked ? 
			"Mouse is currently locked. Right click to see your cursor." : 
			"Right click the window to gain full control of the player."),
			25.0f, HEIGHT - 35.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		textRenderer.RenderText(fontSmall, std::to_string(fps) + " FPS",
			25.0f, HEIGHT - 55.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));	
		
		// If there is dialogue currently, render it
		if (currentDialogue != "")
		{
			// Draw shadow first
			textRenderer.RenderText(fontLarge, currentDialogue,
				WIDTH/2 + 2.5, HEIGHT/2 - 2.5, 1.0f, glm::vec3(0.25f, 0.25f, 0.25f), true);
			
			textRenderer.RenderText(fontLarge, currentDialogue,
				WIDTH/2, HEIGHT/2, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);
		}

		// Swap buffers
		display.Update();
	}

	// Quit SDL...
	SDL_Quit();

	return 0;
}