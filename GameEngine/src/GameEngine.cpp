#include "game_engine_stdafx.h"

#include "GameEngine.h"
#include "ThirdPersonCameraSystem.h"
#include "Player/PlayerSystem.h"

using namespace ECS;
using namespace Utils;
using namespace Core;
using namespace Renderer;
using namespace Camera;

GameEngine::GameEngine(): m_window(), m_scene(nullptr), m_camera(nullptr)
{}

void GameEngine::Init()
{
	/*-----------------DEMO Controller-------------------*/
	/*m_window.StartContext();
	ECS::Initialize();

	m_scene = ECS_Engine->GetSystemManager()->AddSystem<SceneSystem>();
	Physics::PhysicsSystem* PyS = ECS_Engine->GetSystemManager()->AddSystem<Physics::PhysicsSystem>(glm::vec3(0.0f, -9.81f, 0.0f));

	InputSystem* IpS = ECS_Engine->GetSystemManager()->AddSystem<InputSystem>(m_window.GetContext());

	PlayerSystem* PlS = ECS_Engine->GetSystemManager()->AddSystem<PlayerSystem>();
	PlS->SetSpeed(2.0f);

	Physics::Bounds::AABB worldBounds(Maths::vec3f(-100.f, -100.f, -100.f), Core::Maths::vec3f(100.f, 100.f, 100.f));
	Physics::Collision::CollisionSystem* CoS = ECS_Engine->GetSystemManager()->AddSystem<Physics::Collision::CollisionSystem>(worldBounds);

	ECS_Engine->GetSystemManager()->AddSystem<ThirdPersonCameraSystem>();

	RenderSystem* RdS = ECS_Engine->GetSystemManager()->AddSystem<RenderSystem>(m_window.GetContext());

	// Add system dependencies
	PlS->AddDependencies(IpS);
	PyS->AddDependencies(IpS, PlS, m_scene);
	CoS->AddDependencies(IpS, PyS, m_scene);
	RdS->AddDependencies(IpS, CoS, PyS, m_scene);

	ECS_Engine->GetSystemManager()->UpdateSystemWorkOrder();*/

	/*-----------------DEMO Physics / Renderer-------------------*/
	m_window.StartContext();
	ECS::Initialize();

	m_scene = ECS_Engine->GetSystemManager()->AddSystem<SceneSystem>();
	Physics::PhysicsSystem* PyS = ECS_Engine->GetSystemManager()->AddSystem<Physics::PhysicsSystem>(glm::vec3(0.0f, -9.81f, 0.0f));

	Physics::Bounds::AABB worldBounds(Maths::vec3f(-10.f, -10.f, -10.f), Core::Maths::vec3f(10.f, 10.f, 10.f));
	Physics::Collision::CollisionSystem* CoS = ECS_Engine->GetSystemManager()->AddSystem<Physics::Collision::CollisionSystem>(worldBounds);

	RenderSystem* RdS = ECS_Engine->GetSystemManager()->AddSystem<RenderSystem>(m_window.GetContext());

	PyS->AddDependencies(m_scene);
	CoS->AddDependencies(PyS, m_scene);
	RdS->AddDependencies(CoS, PyS, m_scene);

	ECS_Engine->GetSystemManager()->UpdateSystemWorkOrder();

	/*-----------------DEMO Parented Component-------------------*/
	/*m_window.StartContext();
	ECS::Initialize();

	m_scene = ECS_Engine->GetSystemManager()->AddSystem<SceneSystem>();

	RenderSystem* RdS = ECS_Engine->GetSystemManager()->AddSystem<RenderSystem>(m_window.GetContext());

	RdS->AddDependencies(m_scene);

	ECS_Engine->GetSystemManager()->UpdateSystemWorkOrder();*/
}

void GameEngine::Setup()
{
	/*-----------------DEMO Controller-------------------*/
	/*m_camera = &m_scene->AddGameObject<CameraGameObject>();
	ECS_Engine->GetSystemManager()->GetSystem<RenderSystem>()->SetCamera(m_camera);

	Core::Transform& camTransform = m_camera->GetComponent<Core::TransformComponent>()->AsTransform();
	camTransform.SetPosition(glm::vec3(-2.f, 2.f, 10.f));
	ECS_Engine->GetSystemManager()->GetSystem<ThirdPersonCameraSystem>()->SetCamera(m_camera->GetEntityID());*/

	/*-----------------DEMO-------------------*/
	m_camera = &m_scene->AddGameObject<CameraGameObject>();
	ECS_Engine->GetSystemManager()->GetSystem<RenderSystem>()->SetCamera(m_camera);
	
	Core::Transform& camTransform = m_camera->GetComponent<Core::TransformComponent>()->AsTransform();
	camTransform.SetPosition(glm::vec3(-1.0f, 0.0f, 5.0f));
	//camTransform.LookAt(glm::vec3(0.0f, 0.0f, -15.0f));

	//m_camera->UseOrtho(-2, 2, -1.5, 1.5, 1, 20);
}

void GameEngine::Run()
{
	/*-----------------DEMO Controller-------------------*/
	/*Player& player = m_scene->AddGameObject<Player>(
		m_resourcesManager.LoadMesh("sphere", "sphere.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	player.GetRigidbody().Position(Maths::vec3f(-2.0f, 2.0f, 0.0f));
	ECS_Engine->GetSystemManager()->GetSystem<PlayerSystem>()->SetPlayer(&player);
	ECS_Engine->GetSystemManager()->GetSystem<ThirdPersonCameraSystem>()->SetTarget(player.GetEntityID());

	auto& platform = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "centered_cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("paint", "rock.jpg"));

	auto& Wall = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.GetMesh("cube"),
		&m_resourcesManager.GetShader("texture"),
		&m_resourcesManager.GetTexture("paint"));

	auto& platformTransform = platform.GetComponent<Core::TransformComponent>()->AsTransform();
	platform.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-25.0f, -2.5f, -25.0f), Maths::vec3f(25.0f, 2.5f, 25.0f), Transform());
	platformTransform.SetPosition(0.0f, -2.5f, 0.0f);
	platformTransform.Scale(50.0f, 5.0f, 50.0f);

	auto& wallTransform = Wall.GetComponent<Core::TransformComponent>()->AsTransform();
	Wall.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-0.5f, -2.5f, -2.5f), Maths::vec3f(0.5f, 2.5f, 2.5f), Transform());
	wallTransform.SetPosition(1.5f, 2.5f, 0.0f);
	wallTransform.Scale(1.0f, 5.0f, 5.0f);*/

	/*-----------------DEMO Physics / Renderer-------------------*/
	auto& sphere = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("sphere", "sphere.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& cube = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "centered_cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("rock", "rock.jpg"));

	auto& sphereTransform = sphere.GetComponent<Core::TransformComponent>()->AsTransform();
	sphereTransform.SetPosition(0.0f, 1.0f, 0.0f);
	sphere.AddComponent<Physics::Collision::SphereColliderComponent>(0.5f, Transform());
	sphere.AddComponent<Physics::RigidBody::RigidBodyComponent>(1.0f, 1.0f, true);

	auto& cubeTransform = cube.GetComponent<Core::TransformComponent>()->AsTransform();
	cubeTransform.SetPosition(0.0f, -1.0f, 0.0f);
	cube.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-0.5f, -0.5f, -0.5f), Maths::vec3f(0.5f, 0.5f, 0.5f), Transform());
	
	auto& cube2 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "centered_cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& sphere2 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("sphere", "sphere.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("rock", "rock.jpg"));

	auto& cube2Transform = cube2.GetComponent<Core::TransformComponent>()->AsTransform();
	cube2Transform.SetPosition(-2.0f, 1.0f, 0.0f);
	cube2.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-0.5f, -0.5f, -0.5f), Maths::vec3f(0.5f, 0.5f, 0.5f), Transform());
	cube2.AddComponent<Physics::RigidBody::RigidBodyComponent>(1.0f, 1.0f, true);
	
	auto& sphere2Transform = sphere2.GetComponent<Core::TransformComponent>()->AsTransform();
	sphere2Transform.SetPosition(-2.0f, -1.0f, 0.0f);
	sphere2.AddComponent<Physics::Collision::SphereColliderComponent>(0.5f, Transform());

	auto& sphere3 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("sphere", "sphere.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("rock", "rock.jpg"));

	auto& sphere4 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("sphere", "sphere.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& sphere3Transform = sphere3.GetComponent<Core::TransformComponent>()->AsTransform();
	sphere3Transform.SetPosition(2.0f, 1.0f, 0.0f);
	sphere3.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-0.5f, -0.5f, -0.5f), Maths::vec3f(0.5f, 0.5f, 0.5f), Transform());
	sphere3.AddComponent<Physics::RigidBody::RigidBodyComponent>(1.0f, 1.0f, true);

	auto& sphere4Transform = sphere4.GetComponent<Core::TransformComponent>()->AsTransform();
	sphere4Transform.SetPosition(2.0f, -1.0f, 0.0f);
	sphere4.AddComponent<Physics::Collision::SphereColliderComponent>(0.5f, Transform());

	auto& cube3 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("rock", "rock.jpg"));

	auto& cube4 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& cube3Transform = cube3.GetComponent<Core::TransformComponent>()->AsTransform();
	cube3Transform.SetPosition(-4.0f, 1.0f, 0.0f);
	cube3.AddComponent<Physics::Collision::BoxColliderComponent>(Maths::vec3f(-0.5f, -0.5f, -0.5f), Maths::vec3f(0.5f, 0.5f, 0.5f), Transform());
	cube3.AddComponent<Physics::RigidBody::RigidBodyComponent>(1.0f, 1.0f, true);

	auto& cube4Transform = cube4.GetComponent<Core::TransformComponent>()->AsTransform();
	cube4Transform.SetPosition(-4.0f, -1.0f, 0.0f);
	cube4.AddComponent<Physics::Collision::SphereColliderComponent>(0.5f, Transform());

	/*-----------------DEMO Parented Component-------------------*/
	/*auto& cube = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "centered_cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("paint", "rock.jpg"));

	auto& cube2 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.GetMesh("cube"),
		&m_resourcesManager.GetShader("texture"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& cube3 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.LoadMesh("cube", "cube.obj"),
		&m_resourcesManager.LoadShader("texture", "texture.glsl"),
		&m_resourcesManager.LoadTexture("paint", "rock.jpg"));

	auto& cube4 = m_scene->AddGameObject<Renderer::TexturedMeshGameObject>(
		m_resourcesManager.GetMesh("cube"),
		&m_resourcesManager.GetShader("texture"),
		&m_resourcesManager.LoadTexture("copper", "copper.jpg"));

	auto& cubeTransform = cube.GetComponent<Core::TransformComponent>()->AsTransform();
	cubeTransform.SetPosition(0.0f, -1.0f, 0.0f);

	auto& cube2Transform = cube2.GetComponent<Core::TransformComponent>()->AsTransform();
	cube2.AddComponent<ParentedComponent>(cube.GetEntityID());
	cube2Transform.SetPosition(-1.5f, 0.0f, 0.0f);

	auto& cube3Transform = cube3.GetComponent<Core::TransformComponent>()->AsTransform();
	cube3.AddComponent<ParentedComponent>(cube2.GetEntityID());
	cube3Transform.SetPosition(-2.0f, 0.0f, -2.0f);

	auto& cube4Transform = cube4.GetComponent<Core::TransformComponent>()->AsTransform();
	cube4.AddComponent<ParentedComponent>(cube3.GetEntityID());
	cube4Transform.SetPosition(1.5f, 0.0f, 0.0f);*/

	while (!glfwWindowShouldClose(m_window.GetContext()))
	{
		glfwPollEvents();

		Timer::PreUpdate();

		while (Timer::Accumulator() >= Timer::FixedDeltaTime())
		{
			Timer::FixedUpdate();
			ECS_Engine->FixedUpdate(Timer::FixedDeltaTime().count());

			/*cubeTransform.Rotate(1.0f, 1.0f, 1.0f);
			cube2Transform.Rotate(1.0f, 0.0f, 0.0f);
			cube3Transform.Rotate(0.5f, 0.5f, 0.0f);
			cube4Transform.Rotate(0.0f, 0.5f, 2.0f);*/
		}
		ECS_Engine->Update(Timer::DeltaTime().count());
	}
}

void GameEngine::Close() const
{
	ECS::Terminate();
	m_window.Close();
}