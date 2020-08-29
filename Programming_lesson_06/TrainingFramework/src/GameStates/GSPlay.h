#pragma once
#include "gamestatebase.h"

class Sprite2D;
class Sprite3D;
class Text;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

	void DoCollision();
	void RandomSprite();

	void SetNewPostionForBullet();

private:

	std::shared_ptr<Sprite2D>	m_BackGround;
	std::shared_ptr<Text>		m_score;
	std::shared_ptr<Sprite2D>	m_Player;
	std::shared_ptr<Sprite2D>	m_SurfBoard;
	std::vector<std::shared_ptr<Sprite2D>>	m_ArrWater;
	std::vector<std::shared_ptr<Enemy>>	m_ArrObject;
};
