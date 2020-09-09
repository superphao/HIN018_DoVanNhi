#include "GSOption.h"
extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSOption::GSOption()
{

}

GSOption::~GSOption()
{
}

void GSOption::Init()
{
	//background
	glClearColor(56.0f / 255.0f, 194.0f / 255.0f, 238.0f / 255.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_OptionState = OPTION_MAIN;

	//water
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("water256");

	for (int i = 0; i < 4; ++i) {
		m_ArrWater.push_back(std::make_shared<Sprite2D>(model, shader, texture));
	}
	int i = 0, j = 0;
	for (auto& obj : m_ArrWater) {
		obj->SetSize((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2);
		obj->Set2DPosition(i * screenWidth / 2 + (GLfloat)screenWidth / 4, j * screenHeight / 2 + (GLfloat)screenHeight / 4);
		obj->Init();
		i++;
		if (i == 2) {
			i = 0;
			j++;
		}
	}
	
	//back button
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 200);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

	//button sound
	texture = ResourceManagers::GetInstance()->GetTexture("button_sound");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 300);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->SettingSound();
		});
	m_listButton.push_back(button);

	//button help
	texture = ResourceManagers::GetInstance()->GetTexture("button_help");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 400);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->Help();
		});
	m_listButton.push_back(button);

	//button about
	texture = ResourceManagers::GetInstance()->GetTexture("button_about");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 500);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->About();

		});
	m_listButton.push_back(button);

	//button exit game
	texture = ResourceManagers::GetInstance()->GetTexture("button_exit_game");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 600);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		exit(0);
		});
	m_listButton.push_back(button);

	texture = ResourceManagers::GetInstance()->GetTexture("dialog");
	m_Dialog = std::make_shared<GameButton>(model, shader, texture);
	m_Dialog->SetSize(500, 250);
	m_Dialog->Set2DPosition(screenWidth/2, screenHeight/2);

	//buton close
	texture = ResourceManagers::GetInstance()->GetTexture("button_close");
	m_buttonClose = std::make_shared<GameButton>(model, shader, texture);
	m_buttonClose->Set2DPosition(m_Dialog->Get2DPosition().x + 250 - 24 , m_Dialog->Get2DPosition().y - 125 + 24);
	m_buttonClose->SetSize(24, 24);
	m_buttonClose->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->CloseDialog();
		});

	//button add volume
	texture = ResourceManagers::GetInstance()->GetTexture("add");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 + 60, screenHeight/2);
	button->SetSize(32, 32);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->AddVolume();
		});
	m_lButtonVolume.push_back(button);

	//button sub volume
	texture = ResourceManagers::GetInstance()->GetTexture("minus");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 - 30, screenHeight / 2);
	button->SetSize(32, 32);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSOption>(GameStateMachine::GetInstance()->CurrentState())->SubVolume();
		});
	m_lButtonVolume.push_back(button);

	////text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared< Text>(shader, font, "LET'S SURF", TEXT_COLOR::BLACK, 1.5);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 100, 120));

	//text number volume
	m_numVolume = (GLint) m_soloud.getGlobalVolume() * 10;
	m_Text_NumVolume = std::make_shared< Text>(shader, font, std::to_string(m_numVolume), TEXT_COLOR::BLACK, 1.5);
	m_Text_NumVolume->Set2DPosition(screenWidth / 2, screenHeight/2);
}

void GSOption::Exit()
{
	
}

void GSOption::Pause()
{
}

void GSOption::Resume()
{
}

void GSOption::HandleEvents()
{
}

void GSOption::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed)
	{
		switch (key)
		{
		case VK_ESCAPE:
			if (m_OptionState != OPTION_MAIN)
			{
				CloseDialog();
			}
			break;
		default:
			break;
		}
	}
}

void GSOption::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	if (bIsPressed)
	{
		if(m_OptionState == OPTION_MAIN)
			for (auto it : m_listButton)
			{
				(it)->HandleTouchEvents(x, y, bIsPressed);
				if ((it)->IsHandle()) break;
			}
		if (m_OptionState == OPTION_SOUND)
		{
			for(auto &obj : m_lButtonVolume)
			{
				obj->HandleTouchEvents(x, y, bIsPressed);
				if (obj->IsHandle()) break;
			}
			m_buttonClose->HandleTouchEvents(x, y, bIsPressed);
		}
		if (m_OptionState == OPTION_ABOUT || m_OptionState == OPTION_HELP)
		{
			m_buttonClose->HandleTouchEvents(x, y, bIsPressed);
		}
	}
}

void GSOption::Update(float deltaTime)
{
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	m_Text_gameName->Update(deltaTime);
	m_Text_NumVolume->Update(deltaTime);
}

void GSOption::Draw()
{
	for (auto& obj : m_ArrWater)
	{
		obj->Draw();
	}

	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_Text_gameName->Draw();
	if (m_OptionState == OPTION_SOUND)
	{
		m_Dialog->Draw();
		m_buttonClose->Draw();
		m_Text_NumVolume->Draw();
		for (auto& obj : m_lButtonVolume)
		{
			obj->Draw();
		}
	}
	if (m_OptionState == OPTION_ABOUT)
	{
		m_Dialog->Draw();
		m_buttonClose->Draw();
	}
	if (m_OptionState == OPTION_HELP)
	{
		m_Dialog->Draw();
		m_buttonClose->Draw();
	}
}

void GSOption::SettingSound()
{
	m_OptionState = OPTION_SOUND;
}

void GSOption::CloseDialog()
{
	m_OptionState = OPTION_MAIN;
}

void GSOption::About()
{
	m_OptionState = OPTION_ABOUT;
}

void GSOption::Help()
{
	m_OptionState = OPTION_HELP;
}

void GSOption::AddVolume()
{
	if (m_numVolume < 10)
	{
		m_numVolume++;
		m_Text_NumVolume->setText(std::to_string(m_numVolume));
	}
}

void GSOption::SubVolume()
{
	if (m_numVolume > 0)
	{
		m_numVolume--;
		m_Text_NumVolume->setText(std::to_string(m_numVolume));
		m_soloud.stopAll();
	}
}

