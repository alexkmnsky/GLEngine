/** Copyright (c) 2022-2023 Alexander Kaminsky and Maxwell Hunt */

#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

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
#include "Events/Keycode.h"

#include "GameComponentSystem/TransformComponent.h"
#include "GameComponentSystem/ColliderComponent.h"
#include "GameComponentSystem/FreecamControlComponent.h"
#include "GameComponentSystem/RenderableMeshComponentSystem.h"
#include "GameComponentSystem/MotionComponentSystem.h"
#include "GameComponentSystem/CameraComponentSystem.h"

#include "Physics/Components/RigidbodyComponent.h"
#include "Physics/Systems/PhysicsWorldSystem.h"

// Window size
#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

class ImpulseSolverInteraction final : public Interaction
{
public:
	ImpulseSolverInteraction() : Interaction()
	{
		AddInteractorComponentType(TransformComponent::ID);
		AddInteractorComponentType(ColliderComponent::ID);
		AddInteractorComponentType(RigidbodyComponent::ID);
		AddInteracteeComponentType(TransformComponent::ID);
		AddInteracteeComponentType(ColliderComponent::ID);
	}

	void Interact(float deltaTime, EntityHandle interactor, EntityHandle interactee,
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents,
		const CollisionPoints& points) final
	{
		const auto interactorTransform = (TransformComponent*)interactorComponents[0];
		const auto interacteeTransform = (TransformComponent*)interacteeComponents[0];
		const auto interactorCollider  = (ColliderComponent*)interactorComponents[1];
		const auto interacteeCollider  = (ColliderComponent*)interacteeComponents[1];
		const auto interactorRigidbody = (RigidbodyComponent*)interactorComponents[2];

		const glm::vec3 relativeVelocity = -interactorRigidbody->velocity;

		float nSpd = glm::dot(relativeVelocity, points.normal);

		if (nSpd >= 0)
		{
			return;
		}

		float e = interactorRigidbody->restitution; // Coefficient of restitution

		float j = -(1.f + e) * nSpd / ((1.f / interactorRigidbody->mass) + 1.f); // Magnitude of impulse

		glm::vec3 impulse = j * points.normal;

		interactorRigidbody->velocity -= impulse * (1.f / interactorRigidbody->mass);
	}
};

class SmoothPositionSolverInteraction final : public Interaction
{
public:
	SmoothPositionSolverInteraction() : Interaction()
	{
		AddInteractorComponentType(TransformComponent::ID);
		AddInteractorComponentType(ColliderComponent::ID);
		AddInteractorComponentType(RigidbodyComponent::ID);
		AddInteracteeComponentType(TransformComponent::ID);
		AddInteracteeComponentType(ColliderComponent::ID);
	}

	void Interact(float deltaTime, EntityHandle interactor, EntityHandle interactee,
		BaseECSComponent** interactorComponents, BaseECSComponent** interacteeComponents,
		const CollisionPoints& points) final
	{
		const auto interactorTransform = (TransformComponent*)interactorComponents[0];
		const auto interacteeTransform = (TransformComponent*)interacteeComponents[0];
		const auto interactorCollider = (ColliderComponent*)interactorComponents[1];
		const auto interacteeCollider = (ColliderComponent*)interacteeComponents[1];
		const auto interactorRigidbody = (RigidbodyComponent*)interactorComponents[2];

		constexpr float percent = 0.8f;
		constexpr float slop = 0.01f;

		glm::vec3 correction = points.normal * percent * std::max(points.depth - slop, 0.f)
			/ (1.f / interactorRigidbody->mass);

		interactorTransform->transform.GetPosition() -= (1.f / interactorRigidbody->mass) * correction;

	}
};

// TODO refactor main
int main(int argc, char** argv)
{
	Application* application = Application::Create();
	Window window(*application, DEFAULT_WIDTH, DEFAULT_HEIGHT, "GLEngine");
	RenderDevice device(window);
	Sampler sampler(device);

	Shader shader(device, "./Assets/Shaders/BasicShader.glsl");
	Shader shaderText(device, "./Assets/Shaders/TextShader.glsl");

	// Create a camera used for rendering
	Camera camera(70.0f, (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 1000.0f, 
		glm::vec3(0.0f, 0.0f, 10.0f));

	RenderDevice::DrawParameters drawParameters;
	drawParameters.primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
	drawParameters.faceCulling = RenderDevice::FACE_CULL_BACK;
	drawParameters.depthFunc = RenderDevice::DRAW_FUNC_LESS;
	drawParameters.shouldWriteDepth = true;

	RenderTarget target(device);
	GameRenderContext gameRenderContext(device, target, drawParameters, shader, sampler, camera);

	std::vector<IndexedModel> models = LoadModels("./Assets/Models/Sphere.obj");
	VertexArray vertexArray(device, models[0], RenderDevice::USAGE_STATIC_DRAW);

	// Load textures
	Texture textureGreen(device, "./Assets/Textures/Green/texture_09.png",RenderDevice::FORMAT_RGBA,false,false);
	Texture textureRed(device, "./Assets/Textures/Red/texture_09.png",RenderDevice::FORMAT_RGBA,false,false);

	// Create the text renderer and load fonts used
	TextRenderer textRenderer((float)window.GetWidth(), (float)window.GetHeight(), device, target, 
		shaderText, sampler);

	Font* font = textRenderer.LoadFont("./Assets/Fonts/font.ttf", 64);

	std::vector<Text::Layer> style = {
		{ glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f / 16.f, .5f, Transform(glm::vec3(-5.f, -5.f, 0.f)) },
		{ glm::vec4(1.f, 0.f, 0.f, 1.f), 1.f / 3.f, .3f,  Transform() },
		{ glm::vec4(1.f, 0.f, 0.f, 1.f), 1.f / 16.f, .4f, Transform() },
		{ glm::vec4(1.f, 1.f, 1.f, 1.f), 1.f / 16.f, .5f, Transform() },
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

	ImpulseSolverInteraction impulseSolverInteraction;
	SmoothPositionSolverInteraction smoothPositionSolverInteraction;
	interactionWorld.AddInteraction(&impulseSolverInteraction);
	interactionWorld.AddInteraction(&smoothPositionSolverInteraction);

	// Create the event handler for responding to window events
	GameEventHandler eventHandler;

	bool isMouseLocked = false;
	ActionControl lockMouse;
	AxisControl xControl;
	AxisControl yControl;
	MotionControl motionControl;
	eventHandler.AddMouseMotionControl(motionControl, 0.1f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_W, yControl, 10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_S, yControl, -10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_A, xControl, -10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_D, xControl, 10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_UP, yControl, 10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_DOWN, yControl, -10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_LEFT, xControl, -10.f);
	eventHandler.AddKeyAxisControl(Keycode::KEY_RIGHT, xControl, 10.f);
	eventHandler.AddButtonActionControl(3, lockMouse);

	eventHandler.AddWindowResizeCallback(
		[&window, &target, &camera, &textRenderer](unsigned int width, unsigned int height)
		{
			std::cout << "Window was resized to " << width << " " << height << std::endl;
			
			window.ChangeSize(width, height);
			target.UpdateSize(width, height);
			camera.SetAspect((float)width / (float)height);
			textRenderer.UpdateSize(width, height);
		}
	);

	// Create components
	TransformComponent transformComponent;
	ColliderComponent colliderComponent;
	FreecamControlComponent freecamControlComponent;
	CameraComponent cameraComponent;
	RenderableMeshComponent renderableMeshComponent;
	RigidbodyComponent rigidbodyComponent;
	
	// Create the player entity...

	// Set the player position to the start of the maze
	transformComponent.transform.SetPosition(glm::vec3(19.0f, 40.0f, 30.0f));
	// Set up the camera attached to the player
	// No offset; first person
	cameraComponent.camera = &camera;
	cameraComponent.offset = Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	freecamControlComponent.forward = &yControl;
	freecamControlComponent.right = &xControl;
	freecamControlComponent.motion = &motionControl;

	// Set mesh and texture for the flag

	colliderComponent.aabb = AABB(glm::vec3(-1), glm::vec3(1));

	SphereCollider myCollider;
	myCollider.center = glm::vec3(0);
	myCollider.radius = 1;
	colliderComponent.collider = myCollider;

	// Finally, create the player!
	ecs.MakeEntity(transformComponent, cameraComponent, freecamControlComponent);

	renderableMeshComponent.mesh = &vertexArray;
	renderableMeshComponent.texture = &textureRed;

	constexpr float spacing = 5.f;
	for (unsigned int i = 0; i < 10; i++)
	{
		for(unsigned int j = 0;j < 10;j++)
		{
			transformComponent.transform.SetPosition(glm::vec3(spacing * j + (i%2) * 2.5f, spacing * i, -18.f));
			ecs.MakeEntity(transformComponent, colliderComponent, renderableMeshComponent);
		}
	}

	transformComponent.transform.SetPosition(glm::vec3(19.0f, 50.0f, -18.0f));

	rigidbodyComponent.velocity = glm::vec3(0.f);
	rigidbodyComponent.force = glm::vec3(0.f);
	rigidbodyComponent.mass = .5f;
	rigidbodyComponent.takesGravity = true;
	rigidbodyComponent.dynamicFriction = 0.f;
	rigidbodyComponent.staticFriction = 0.f;
	rigidbodyComponent.restitution = 1.f;
	renderableMeshComponent.texture = &textureGreen;
	ecs.MakeEntity(transformComponent, colliderComponent, rigidbodyComponent, renderableMeshComponent);

	// Create systems
	PhysicsWorldSystem physicsWorldSystem;
	FreecamControlSystem freecamControlSystem;
	CameraSystem cameraSystem;
	RenderableMeshSystem renderableMeshSystem(gameRenderContext);

	mainSystems.AddSystem(physicsWorldSystem);
	mainSystems.AddSystem(freecamControlSystem);
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

		while (!lockMouse.IsEmpty())
		{
			if (lockMouse.Pop() == ActionControl::PRESS)
			{
				if (isMouseLocked)
				{
					application->UnlockMouse();
					isMouseLocked = false;
				}
				else
				{
					application->LockMouse();
					isMouseLocked = true;
				}
			}
		}

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
		textTransform.SetPosition(glm::vec3(window.GetWidth() / 2.f, window.GetHeight() / 5.f, 0));
		textTransform.SetRotation(glm::vec3(0.f, 0.f, sin(Timing::GetTime()) * 3.f));
		textTransform.SetScale(glm::vec3(-abs(sin(Timing::GetTime() * 2)) / 6 + 0.9));
		hwText.SetTransform(textTransform);

		hwText.SetText("Game Engine Physics Demo");

		textRenderer.RenderText(hwText);

		// Swap buffers
		window.Present();
	}

	delete application;

	return 0;
}