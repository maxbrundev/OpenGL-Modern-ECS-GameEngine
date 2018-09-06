#include "game_engine_stdafx.h"

#include "Player/PlayerSystem.h"

PlayerSystem::PlayerSystem()
	: m_speed(0.5f), m_maxSpeed(5.0f)
{
	RegisterEventCallback(&PlayerSystem::OnInputMove);
}
PlayerSystem::~PlayerSystem()
{
	UnregisterAllEventCallbacks();
}

void PlayerSystem::FixedUpdate(double fixed_dt)
{
	if (m_player->GetRigidbody().Velocity() > m_maxSpeed)
	{
		m_player->GetRigidbody().Velocity().Normalize();
		m_player->GetRigidbody().Velocity() *= m_maxSpeed;
	}
}

void PlayerSystem::OnInputMove(const Core::Events::KeyPressed* kp)
{
	switch (kp->keycode)
	{
	case Core::Events::KeyCode::KEYCODE_W:
		MoveForward(m_speed);
		break;
	case Core::Events::KeyCode::KEYCODE_S:
		MoveBackward(m_speed);
		break;
	case Core::Events::KeyCode::KEYCODE_D:
		MoveRight(m_speed);
		break;
	case Core::Events::KeyCode::KEYCODE_A:
		MoveLeft(m_speed);
		break;
	case Core::Events::KeyCode::KEYCODE_SPACE:
		Jump(15.0f);
		break;
	default:
		break;
	}
}

void PlayerSystem::MoveForward(const float p_speed) const
{
	if (m_player)
	{
		m_player->GetRigidbody().Velocity() +=  p_speed * m_player->GetTransform().Forward();	
	}
}

void PlayerSystem::MoveBackward(const float p_speed) const
{
	if (m_player)
	{
		m_player->GetRigidbody().Velocity() += p_speed * -m_player->GetTransform().Forward();
	}
}

void PlayerSystem::MoveRight(const float p_speed) const
{
	if (m_player)
	{
		m_player->GetRigidbody().Velocity() += p_speed * m_player->GetTransform().Right();
	}
}

void PlayerSystem::MoveLeft(const float p_speed) const
{
	if (m_player)
	{
		m_player->GetRigidbody().Velocity() += p_speed * -m_player->GetTransform().Right();

	}
}

void PlayerSystem::Jump(const float p_height) const
{
	if (m_player)
	{
		m_player->GetRigidbody().Addforce(glm::vec3(0, p_height, 0));
	}
}

void PlayerSystem::SetPlayer(Player* p_player)
{
	if(!m_player)
		m_player = p_player;
}

void PlayerSystem::SetSpeed(const float p_speed)
{
	m_speed = p_speed;
}

void PlayerSystem::SetMaxSpeed(float p_maxSpeed)
{
	m_maxSpeed = p_maxSpeed;
}