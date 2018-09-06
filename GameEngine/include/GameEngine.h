#pragma once

#include "game_engine_stdafx.h"

class GameEngine : protected ECS::Event::IEventListener
{
private:
	Renderer::Context::Window				m_window;
	Core::SceneSystem*						m_scene;
	Renderer::CameraGameObject*				m_camera;
	Renderer::Resources::ResourcesManager	m_resourcesManager;

public:
	GameEngine();
	~GameEngine() = default;

	void Init();
	void Setup();
	void Run();
	void Close() const;
};