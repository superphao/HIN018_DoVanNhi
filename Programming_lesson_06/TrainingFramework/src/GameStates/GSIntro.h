#pragma once
#include "gamestatebase.h"

class GSIntro :
	public GameStateBase
{
public:
	GSIntro();
	~GSIntro();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);
	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

private:
	std::shared_ptr<Sprite2D> m_enemy;
	std::shared_ptr<Player> m_Player;
	float m_time;
	std::vector<std::shared_ptr<DynamicSprite>>	m_ArrWater;
	
};

