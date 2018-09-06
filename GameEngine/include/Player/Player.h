#pragma once

#include "game_engine_stdafx.h"

using namespace Renderer;

class Player : public Core::GameObject<Player>
{
private:
	Core::Transform*							m_transform;
	Physics::RigidBody::RigidBodyComponent*		m_rigidbody;

public :
	Player(GAMEOBJECT_INIT_PARAMS, Resources::Mesh& p_mesh, Resources::Shader* p_shader, Resources::Texture* p_texture);
	~Player();

	Core::Transform& GetTransform() const;
	Physics::RigidBody::RigidBodyComponent& GetRigidbody() const;
};