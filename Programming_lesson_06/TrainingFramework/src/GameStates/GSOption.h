#pragma once
#include "GameStatebase.h"
#include "GameButton.h"
#include "Application.h"

enum OptionState
{
	OPTION_SOUND,
	OPTION_ABOUT,
	OPTION_HELP,
	OPTION_MAIN,
};

enum NameText
{
	TEXT_NAME_GAME,
	TEXT_TITLE_DIALOG,
	TEXT_MUSIC,
	TEXT_NUM_VOLUME_MUSIC,
	TEXT_SOUND_EFFECTS,
	TEXT_NUM_VOLUME_EFFECT,
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
	void AddVolumeMusic();
	void SubVolumeMusic();
	void AddVolumeEffect();
	void SubVolumeEffect();
	void SetVolume();

private:
	std::vector< std::shared_ptr<Sprite2D>> m_ArrWater;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	OptionState m_OptionState;
	std::shared_ptr<Sprite2D> m_Dialog;
	std::shared_ptr<Sprite2D> m_DialogHelp;
	std::shared_ptr<Sprite2D> m_DialogAbout;
	std::shared_ptr<GameButton>	m_buttonClose;
	GLint m_numVolumeMusic;
	GLint m_numVolumeEffect;
	std::vector<std::shared_ptr<GameButton>> m_lButtonVolume;
	std::vector<std::shared_ptr<Text>> m_ListText;
};

