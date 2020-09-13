#include "GSMenu.h"
#include "GSPlay.h"
#include <fstream>
#include "GameConfig.h"



extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSMenu::GSMenu()
{

}


GSMenu::~GSMenu()
{
}



void GSMenu::Init()
{

	//background
	glClearColor(56.0f / 255.0f, 194.0f / 255.0f, 238.0f / 255.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	isUpdateState = false;

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

	//lisr player
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	for (int i = 0; i < 9; i++) 
	{
		m_ListPlayer.push_back(std::make_shared<Player>(model, shader, texture));
	}
	i = 0;
	for(auto &obj : m_ListPlayer)
	{

		obj->SetSize(64, 64);
		obj->Set2DPosition(i * 96 + (GLfloat)screenWidth / 2, 240);
		obj->SetNumFrame(9);
		obj->SetNumSprite(13);
		obj->SetFrame(Vector2(3 , i));

		texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
		std::shared_ptr<Sprite2D> surfBoard = std::make_shared<Sprite2D>(model, shader, texture);
		surfBoard->SetSize(64, 64);
		surfBoard->Set2DPosition(i * 96 + (GLfloat)screenWidth / 2, 240);
		surfBoard->SetNumFrame(3);
		surfBoard->SetNumSprite(13);
		surfBoard->SetFrame(Vector2(3, 2));
		surfBoard->Init();
		obj->SetSurfBoard(surfBoard);
		obj->Init();
		i++;
	}

	//heart
	for (int i = 0; i < 3; ++i)
	{
		texture = ResourceManagers::GetInstance()->GetTexture("interface24");
		std::shared_ptr<Sprite2D> heart = std::make_shared<Sprite2D>(model, shader, texture);
		heart->SetSize(24, 24);
		heart->Set2DPosition((GLfloat)screenWidth / 2 - 100 + i * 24, 24);
		heart->SetNumFrame(4);
		heart->SetNumSprite(2);
		heart->SetFrame(Vector2(1, 3));
		heart->Init();
		m_ListHeart.push_back(heart);
	}

	//energy
	for (int i = 0; i < 3; ++i)
	{
		texture = ResourceManagers::GetInstance()->GetTexture("interface24");
		std::shared_ptr<Sprite2D> energy = std::make_shared<Sprite2D>(model, shader, texture);
		energy->SetSize(24, 24);
		energy->Set2DPosition((GLfloat)screenWidth / 2 + 80 + i * 24, 24);
		energy->SetNumFrame(4);
		energy->SetNumSprite(2);
		energy->SetFrame(Vector2(1, 2));
		energy->Init();
		m_ListEnergy.push_back(energy);
	}


	//arow left
	texture = ResourceManagers::GetInstance()->GetTexture("arrow");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition((GLfloat)screenWidth / 2 - 48, 240);
	button->SetSize(32, 32);
	button->SetNumSprite(2);
	button->SetFrame(Vector2(0, 0));
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSMenu>(GameStateMachine::GetInstance()->CurrentState())->ChangePlayerLeft();
		});
	m_listButton.push_back(button);

	//arow right
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition((GLfloat)screenWidth / 2 + 48, 240);
	button->SetSize(32, 32); 
	button->SetNumSprite(2);
	button->SetFrame(Vector2(1, 0));
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSMenu>(GameStateMachine::GetInstance()->CurrentState())->ChangePlayerRight();
		});
	m_listButton.push_back(button);

	//menu setting
	texture = ResourceManagers::GetInstance()->GetTexture("settings");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition((GLfloat) screenWidth - 20, 20);
	button->SetSize(32, 32);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Option);
		});
	m_listButton.push_back(button);

	//crown
	texture = ResourceManagers::GetInstance()->GetTexture("crown");
	m_crown = std::make_shared<Sprite2D>(model, shader, texture);
	m_crown->Set2DPosition((GLfloat)screenWidth / 2 - 10, 20);
	m_crown->SetSize(16, 16);

	////text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared< Text>(shader, font, "LET'S SURF", TEXT_COLOR::BLACK, 1.5);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 100, 100));

	//text description
	font = ResourceManagers::GetInstance()->GetFont("arial");
	std::string gameDecription = "Use A D to select a surfer and SPACEBAR to surfing";
	m_Text_gameDecription = std::make_shared< Text>(shader, font, gameDecription, TEXT_COLOR::BLACK, 1.0);
	m_Text_gameDecription->Set2DPosition(Vector2(screenWidth / 2 - 280, 150));

	//open file highscore
	int buffer[1];
	std::fstream file("..\\Data\\Highscore\\Highscore.bin", std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		LOGI("Load File:Highscore.bin\t\t");
		file.read(reinterpret_cast<char*>(buffer), sizeof(GLint));
	}
	else LOGE("ERROR Highscore.bin \n");
	file.close();
	buffer[0] < 0 ? m_highscore = 0 : m_highscore = buffer[0];

	//text high score
	font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_score = std::make_shared< Text>(shader, font, std::to_string(m_highscore) + "m", TEXT_COLOR::BLACK, 0.7);
	m_Text_score->Set2DPosition(Vector2((GLfloat)screenWidth / 2, 30));

	//theme music
	m_ThemeMusic.load("..\\Data\\Music\\theme_music_cut.wav");
	m_ThemeMusic.setLooping(true);
	Application::GetInstance()->m_soloudMusic.play(m_ThemeMusic);

	//button music
	m_ButtonMusic.load("..\\Data\\Music\\Water_dripping.wav");
	
}

void GSMenu::Exit()
{
}


void GSMenu::Pause()
{

}

void GSMenu::Resume()
{

}


void GSMenu::HandleEvents()
{

}

void GSMenu::HandleKeyEvents(int key, bool bIsPressed)
{
	if(bIsPressed)
	{
		switch (key)
		{
		case VK_SPACE:
			for (auto& obj : m_ListPlayer)
			{
				if (obj->Get2DPosition().x == (GLfloat)screenWidth / 2)
				{
					//lay vi phi cua player da chon trong player texture
					DefautPlayer = (GLint) (obj->GetFrame().y);
					break;
				}
			}
			Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
			m_ThemeMusic.stop();
			isUpdateState = true;
			GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
			break;
		case 'D':
			Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
			ChangePlayerRight();
			break;
		case 'A':
			Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
			ChangePlayerLeft();
			break;
		default:
			break;
		}
	}

}

void GSMenu::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	if (bIsPressed)
	{
		for (auto it : m_listButton)
		{
			(it)->HandleTouchEvents(x, y, bIsPressed);
			if ((it)->IsHandle())
			{
				Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
				break;
			}
		}
	}
}

void GSMenu::Update(float deltaTime)
{

	for (auto& obj : m_ListPlayer)
	{
		if (obj->GetSurfBoard() != nullptr)
		{
			obj->GetSurfBoard()->Animation(deltaTime);
		}
		if (obj->Get2DPosition().x == (GLfloat)screenWidth / 2)
		{
			obj->MenuPlayerAnimation(deltaTime);
		}
	}
	//m_BackGround->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}

	//update music + highscore
	if (isUpdateState)
	{
		int buffer[1];
		std::fstream file("..\\Data\\Highscore\\Highscore.bin", std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			LOGI("Load File:Highscore.bin\t\t");
			file.read(reinterpret_cast<char*>(buffer), sizeof(GLint));
		}
		else LOGE("ERROR Highscore.bin \n");
		file.close();
		buffer[0] < 0 ? m_highscore = 0 : m_highscore = buffer[0];
		isUpdateState = false;
		m_Text_score->setText(std::to_string(m_highscore) + "m");
		Application::GetInstance()->m_soloudMusic.play(m_ThemeMusic);
	}
}

void GSMenu::Draw()
{
	for (auto& obj : m_ArrWater)
	{
		obj->Draw();
	}
	for (auto& obj : m_ListHeart) {
		obj->Draw();
	}
	for (auto& obj : m_ListEnergy) {
		obj->Draw();
	}

	m_crown->Draw();
	m_Text_score->Draw();
	m_Text_gameName->Draw();
	m_Text_gameDecription->Draw();

	for (auto& obj : m_ListPlayer)
	{
		if(obj->GetSurfBoard() != nullptr)
			obj->GetSurfBoard()->Draw();
		obj->Draw();
	}

	for (auto it : m_listButton)
	{
		it->Draw();
	}
}

void GSMenu::ChangePlayerLeft()
{
	if (m_ListPlayer.front()->Get2DPosition().x < GLfloat(screenWidth) / 2)
	{
		for (auto& obj : m_ListPlayer)
		{
			obj->Set2DPosition(obj->Get2DPosition().x + 96, obj->Get2DPosition().y);
			obj->GetSurfBoard()->Set2DPosition(obj->Get2DPosition());
		}
	}
}

void GSMenu::ChangePlayerRight()
{
	if (m_ListPlayer.back()->Get2DPosition().x > GLfloat(screenWidth) / 2)
	{
		for (auto& obj : m_ListPlayer)
		{
			obj->Set2DPosition(obj->Get2DPosition().x - 96, obj->Get2DPosition().y);
			obj->GetSurfBoard()->Set2DPosition(obj->Get2DPosition());
		}
	}
}
