#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "soloud_wav.h"
#include "Application.h"

class Sprite2D;
class Sprite3D;
class Text;

enum StateGamePlay
{
	GAME_PAUSE,
	GAME_OVER,
	GAME_RUNNING,
	GAME_OPTION,
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
	void GameOption();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

	void DoCollision();
	void RandomSprite();
	void RandomItem();

	void SetNewPostionForBullet();

private:

	std::shared_ptr<Text> m_Text_score;
	std::shared_ptr<Player>	m_Player;
	std::vector<std::shared_ptr<DynamicSprite>>	m_ArrWater;
	std::vector<std::shared_ptr<DynamicSprite>>	m_ArrObject;
	StateMoveSprite m_StateMove;
	StatePlayer m_StatePlayer;
	StateGamePlay m_SttGamePlay;
	StateSprite m_StateSprite;
	std::vector<std::shared_ptr<Sprite2D>> m_ListHeart;
	std::vector<std::shared_ptr<Sprite2D>> m_ListEnergy;
	GLboolean m_isPause;
	std::shared_ptr<Text> m_TextSttGame;
	std::shared_ptr<Text> m_DesciptSttGame;
	GLfloat m_TimeBornObj;
	GLfloat m_TimeBornItem;
	GLfloat m_score;
	GLfloat m_SpeedMove;
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::shared_ptr<Sprite2D> m_BackgroundOption;
	std::shared_ptr<GameButton> m_buttonOption;
	GLint m_highscore;
	SoLoud::Wav m_ThemeMusic;
	SoLoud::Wav m_SlowdownMusic;
	SoLoud::Wav m_CollisionsMusic;
	SoLoud::Wav m_GameOverMusic;
	SoLoud::Wav m_colWhirlPoolMusic;
	SoLoud::Wav m_ButtonMusic;
	SoLoud::Wav m_plJumpMusic;
	SoLoud::Wav m_itemMusic;
};

