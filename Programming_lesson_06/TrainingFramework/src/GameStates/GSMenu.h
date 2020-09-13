#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "soloud_wav.h"
#include "soloud_file.h"
#include "Application.h"

class GSMenu :
	public GameStateBase
{
public:
	GSMenu();
	~GSMenu();
	
	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);
	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();
	void ChangePlayerLeft();
	void ChangePlayerRight();

private:
	std::shared_ptr<Sprite2D> m_BackGround;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::shared_ptr<Text>  m_Text_gameName;
	std::shared_ptr<Text>  m_Text_gameDecription;
	std::vector< std::shared_ptr<Sprite2D>> m_ArrWater;
	std::vector< std::shared_ptr<Player>> m_ListPlayer;
	std::shared_ptr<Sprite2D> m_ArrowLeft;
	std::shared_ptr<Sprite2D> m_ArrowRight;
	std::vector<std::shared_ptr<Sprite2D>> m_ListHeart;
	std::vector<std::shared_ptr<Sprite2D>> m_ListEnergy;
	std::shared_ptr<Text>  m_Text_score;
	GLint m_highscore;
	std::shared_ptr<Sprite2D> m_crown;
	GLboolean isUpdateState;
	SoLoud::Wav m_ThemeMusic;
	SoLoud::Wav m_ButtonMusic;
};

