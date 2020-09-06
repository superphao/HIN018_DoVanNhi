#pragma once
#include "gamestatebase.h"

class Sprite2D;
class Sprite3D;
class Text;

enum StateGamePlay
{
	GAME_PAUSE,
	GAME_OVER,
	GAME_RUNNING,
};

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
	void GameOver();
	void ReStart();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

	void DoCollision();
	void RandomSprite();

	void SetNewPostionForBullet();

private:

	std::shared_ptr<Sprite2D> m_BackGround;
	std::shared_ptr<Text> m_score;
	std::shared_ptr<Player>	m_Player;
	std::vector<std::shared_ptr<DynamicSprite>>	m_ArrWater;
	std::vector<std::shared_ptr<DynamicSprite>>	m_ArrObject;
	StateMoveSprite m_StateMove;
	StatePlayer m_StatePlayer;
	StateGamePlay m_SttGamePlay;
	StateSprite m_StateSprite;
	std::vector<std::shared_ptr<Sprite2D>> m_ListHeart;
	GLboolean m_isPause;
	std::shared_ptr<Text> m_TextSttGame;
	std::shared_ptr<Text> m_DesciptSttGame;
	GLfloat m_TimeBornObj;
	GLfloat m_TextScore;
	GLfloat m_SpeedMove;
};

