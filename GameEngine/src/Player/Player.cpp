#include "game_engine_stdafx.h"

#include <Player/Player.h>
#include <RigidBodyComponent.h>

using namespace Renderer;

Player::Player(GAMEOBJECT_INIT_PARAMS, Resources::Mesh& p_mesh, Resources::Shader* p_shader, Resources::Texture* p_texture): GAMEOBJECT_INIT(Player)
{
	m_rigidbody = &this->AddComponent<Physics::RigidBody::RigidBodyComponent>(1.0f, 0.0f, true);
	m_transform = &this->GetComponent<Core::TransformComponent>()->AsTransform();
	
	this->AddComponent<Physics::Collision::SphereColliderComponent>(0.5f, *m_transform);
	this->AddComponent<MeshComponent>(&p_mesh);
	this->AddComponent<MaterialComponent>(p_shader, p_texture);
}

Player::~Player()
{}

Core::Transform& Player::GetTransform() const
{
	return  *m_transform;
}

Physics::RigidBody::RigidBodyComponent& Player::GetRigidbody() const
{
	return *m_rigidbody;
}