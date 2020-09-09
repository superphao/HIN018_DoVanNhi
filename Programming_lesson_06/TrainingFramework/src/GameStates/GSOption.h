#pragma once
#include "GameStatebase.h"
#include "GameButton.h"

enum OptionState
{
	OPTION_SOUND,
	OPTION_ABOUT,
	OPTION_HELP,
	OPTION_MAIN,
};

class GSOption :
    public GameStateBase
{
public:
	GSOption();
	~GSOption();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);
	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

	void SettingSound();
	void CloseDialog();
	void About();
	void Help();
	void AddVolume();
	void SubVolume();

private:
	std::vector< std::shared_ptr<Sprite2D>> m_ArrWater;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::shared_ptr<Text>  m_Text_gameName;
	OptionState m_OptionState;
	std::shared_ptr<Sprite2D> m_Dialog;
	std::shared_ptr<GameButton>	m_buttonClose;
	std::shared_ptr<Text>  m_Text_NumVolume;
	GLint m_numVolume;
	std::vector<std::shared_ptr<GameButton>> m_lButtonVolume;
};

