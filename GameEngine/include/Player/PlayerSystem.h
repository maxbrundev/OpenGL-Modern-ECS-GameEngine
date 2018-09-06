#pragma once

#include "game_engine_stdafx.h"

#include <Player/Player.h>

class PlayerSystem : public ECS::System<PlayerSystem>, ECS::Event::IEventListener
{
private:
	Player* m_player {nullptr};
	float m_speed;
	float m_maxSpeed;

public:
	PlayerSystem();
	~PlayerSystem();

	void FixedUpdate(double fixed_dt) override;

	void OnInputMove(const Core::Events::KeyPressed* kp);

	void MoveForward(float p_speed) const;
	void MoveBackward(float p_speed) const;
	void MoveRight(float p_speed) const;
	void MoveLeft(float p_speed) const;
	void Jump(float p_height) const;

	void SetPlayer(Player* p_player);
	void SetSpeed(float p_speed);
	void SetMaxSpeed(float p_maxSpeed);
};