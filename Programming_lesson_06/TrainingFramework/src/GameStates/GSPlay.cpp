#include "GSPlay.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	glClearColor(56.0f / 255.0f, 194.0f / 255.0f, 238.0f / 255.0f, 0.0f);
	//glClearColor(46.0f / 255.0f, 195.0f / 255.0f, 208.0f / 255.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));
	m_StateMove = FREEZE;
	m_StateSprite = SPEEDUP;
	m_SttGamePlay = GAME_RUNNING;
	m_StatePlayer = PLAYER_NORMAL;
	m_TimeBornObj = 0;
	m_TimeBornItem = 0;
	m_score = 0;
	m_SpeedMove = 6;
	m_isPause = false;

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	//player
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	shader = ResourceManagers::GetInstance()->GetShader("ShadowShader");
	m_Player = std::make_shared<Player>(model, shader, texture);
	m_Player->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	m_Player->SetSize(64, 64);
	m_Player->SetNumFrame(9);
	m_Player->SetNumSprite(13);
	m_Player->SetFrame(Vector2(8, DefautPlayer));
	m_Player->Init();
	m_Player->SetName("player");

	//surfboard
	texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
	std::shared_ptr<Sprite2D> surfBoard = std::make_shared<Sprite2D>(model, shader, texture);
	surfBoard->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	surfBoard->SetSize(64, 64);
	surfBoard->SetNumFrame(3);
	surfBoard->SetNumSprite(13);
	surfBoard->SetFrame(Vector2(8, 2));
	surfBoard->Init();
	m_Player->SetSurfBoard(surfBoard);

	//shadow
	texture = ResourceManagers::GetInstance()->GetTexture("shadow");
	std::shared_ptr<Sprite2D> shadow = std::make_shared<Sprite2D>(model, shader, texture);
	shadow->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 90));
	shadow->SetColorAlpha(0.8);
	shadow->SetSize(48, 48);
	shadow->Init();
	m_Player->SetShadow(shadow);

	//Water
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	texture = ResourceManagers::GetInstance()->GetTexture("water256");
	for (int i = 0; i < 16; ++i) {
		m_ArrWater.push_back(std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove));
	}
	int i = 0, j = 0;
	for (auto& obj : m_ArrWater) {
		obj->SetSize((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2);
		obj->Set2DPosition(i * screenWidth / 2 - (GLfloat)screenWidth / 4, j * screenHeight / 2 - (GLfloat)screenHeight / 4);
		obj->SetName("water");
		obj->Init();
		i++;
		if (i == 4) {
			i = 0;
			j++;
		}
	}

	//object
	texture = ResourceManagers::GetInstance()->GetTexture("island1280");
	std::shared_ptr<DynamicSprite> object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
	object->SetSize(1280, 512);
	object->Set2DPosition(screenWidth/2, 80);
	object->Init();
	m_ArrObject.push_back(object);

	//item energy
	for (int i = 0; i < 4; ++i)
	{
		texture = ResourceManagers::GetInstance()->GetTexture("interact64");
		std::shared_ptr<DynamicSprite> item = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		item->SetName("energy");
		item->SetSize(64, 64);
		item->Set2DPosition(screenWidth / 2 - 160 + i * 96, screenHeight / 2 - 20);
		item->SetNumFrame(4);
		item->SetNumSprite(8);
		item->SetFrame(Vector2(2, 0));
		item->Init();

		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		std::shared_ptr<DynamicSprite> effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition(item->Get2DPosition());
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		effect->Init();
		item->SetEffects(effect);
		item->Init();
		m_ArrObject.push_back(item);
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
		energy->Set2DPosition((GLfloat)screenWidth / 2 + 50 + i * 24, 24);
		energy->SetNumFrame(4);
		energy->SetNumSprite(2);
		energy->SetFrame(Vector2(0, 2));
		energy->Init();
		m_ListEnergy.push_back(energy);
	}

	//menu setting
	texture = ResourceManagers::GetInstance()->GetTexture("settings");
	m_buttonOption = std::make_shared<GameButton>(model, shader, texture);
	m_buttonOption->Set2DPosition((GLfloat)screenWidth - 20, 20);
	m_buttonOption->SetSize(32, 32);
	m_buttonOption->SetOnClick([]() {
		std::dynamic_pointer_cast<GSPlay>(GameStateMachine::GetInstance()->CurrentState())->GameOption();
		});

	//button resume
	texture = ResourceManagers::GetInstance()->GetTexture("button_resume");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 200);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSPlay>(GameStateMachine::GetInstance()->CurrentState())->Resume();
		});
	m_listButton.push_back(button);

	//button restart
	texture = ResourceManagers::GetInstance()->GetTexture("button_restart");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 300);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		std::dynamic_pointer_cast<GSPlay>(GameStateMachine::GetInstance()->CurrentState())->ReStart();
		});
	m_listButton.push_back(button);


	//button quit
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, 400);
	button->SetSize(200, 70);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

	//background option
	shader = ResourceManagers::GetInstance()->GetShader("ColorShader");
	m_BackgroundOption = std::make_shared<Sprite2D>(model, shader, Vector4(46.0f / 255.0f, 195.0f / 255.0f, 208.0f / 255.0f, 0.4f));
	m_BackgroundOption->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2);
	m_BackgroundOption->SetSize(screenWidth, screenHeight);
	m_BackgroundOption->Init();

	//score
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_score = std::make_shared< Text>(shader, font, "0m" , TEXT_COLOR::BLACK, 0.7);
	m_Text_score->Set2DPosition(Vector2((GLfloat) screenWidth/2 - 10, 30));

	//text state game play
	font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_TextSttGame = std::make_shared< Text>(shader, font, "SURF PAUSED", TEXT_COLOR::BLACK, 1.5);
	m_TextSttGame->Set2DPosition(Vector2((GLfloat)screenWidth / 2 - 110, 100));

	//text decription state game
	font = ResourceManagers::GetInstance()->GetFont("arial");
	m_DesciptSttGame = std::make_shared< Text>(shader, font, "SPACEBAR to resume surfing", TEXT_COLOR::BLACK, 0.8);
	m_DesciptSttGame->Set2DPosition(Vector2((GLfloat)screenWidth / 2 - 110, 140));

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

	//theme music
	m_ThemeMusic.load("..\\Data\\Music\\music_theme_play1.wav");
	m_ThemeMusic.setLooping(true);
	Application::GetInstance()->m_soloudMusic.play(m_ThemeMusic);

	//slowdown music
	m_SlowdownMusic.load("..\\Data\\Music\\slowdown.wav");

	//collisions music
	m_CollisionsMusic.load("..\\Data\\Music\\collisions.wav");

	//game over music
	m_GameOverMusic.load("..\\Data\\Music\\gameOver.wav");

	//collisions whirlpool music
	m_colWhirlPoolMusic.load("..\\Data\\Music\\whirlpool.wav");

	//button music
	m_ButtonMusic.load("..\\Data\\Music\\Water_dripping.wav");

	//player jump music
	m_plJumpMusic.load("..\\Data\\Music\\jump.wav");

	//player pick item music
	m_itemMusic.load("..\\Data\\Music\\item.wav");
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{
	m_SttGamePlay = GAME_PAUSE;
	m_isPause = true;
}

void GSPlay::Resume()
{
	m_SttGamePlay = GAME_RUNNING;
	m_isPause = false;
}

void GSPlay::GameOver()
{
	Application::GetInstance()->m_soloudEffects.play(m_GameOverMusic);
	m_SttGamePlay = GAME_OVER;
	m_TextSttGame->setText("SURF AGAIN");
	m_DesciptSttGame->setText("SPACEBAR to surf again");
	m_isPause = true;
	if (m_score > m_highscore)
	{
		int buffer[] = { m_score };
		std::fstream file("..\\Data\\Highscore\\Highscore.bin", std::ios::out | std::ios::binary);
		if (file.is_open())
		{
			LOGI("Load File:Highscore.bin\t\t");
			file.write(reinterpret_cast<char*>(buffer), sizeof(GLint));
		}
		else LOGE("ERROR Highscore.txt \n");
		file.close();
	}
}

void GSPlay::ReStart()
{
	GameStateMachine::GetInstance()->PopState();
	GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
}

void GSPlay::GameOption()
{
	m_SttGamePlay = GAME_OPTION;
	m_isPause = true;
}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed) {
		switch (key) {
		case 'S':
			if (m_SttGamePlay == GAME_RUNNING)
			{
				m_Player->SetFrame(Vector2(3, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(3, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateMove = MOVE_FORWARD;
				if (m_StatePlayer == PLAYER_DIE)
					m_StatePlayer = PLAYER_NORMAL;
			}
			break;
		case 'W':
			if (m_SttGamePlay == GAME_RUNNING)
			{
				if (m_StatePlayer == PLAYER_JUMP)
				{
					m_Player->SetFrame(Vector2(11, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(11, m_Player->GetSurfBoard()->GetFrame().y));
				}
				else
				{
					m_Player->SetFrame(Vector2(0, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(0, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = FREEZE;
				}
			}
			break;
		case 'A':
			if (m_SttGamePlay == GAME_RUNNING)
			{
				if (m_StatePlayer == PLAYER_JUMP)
				{
					m_Player->SetFrame(Vector2(10, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(10, m_Player->GetSurfBoard()->GetFrame().y));
				}
				else
				{
					if (m_StateMove == MOVE_LEFT) {
						m_Player->SetFrame(Vector2(1, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(1, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_LEFT_PLUS;
					}
					else if (m_StateMove == MOVE_LEFT_PLUS) {
						m_Player->SetFrame(Vector2(1, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(1, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_LEFT_PLUS;
					}
					else {
						m_Player->SetFrame(Vector2(2, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(2, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_LEFT;
					}
					if (m_StatePlayer == PLAYER_DIE)
						m_StatePlayer = PLAYER_NORMAL;
				}
			}
			break;
		case 'D':

			if (m_SttGamePlay == GAME_RUNNING)
			{
				if (m_StatePlayer == PLAYER_JUMP)
				{
					m_Player->SetFrame(Vector2(12, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(12, m_Player->GetSurfBoard()->GetFrame().y));
				}
				else
				{
					if (m_StateMove == MOVE_RIGHT) {
						m_Player->SetFrame(Vector2(5, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(5, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_RIGHT_PLUS;
					}
					else if (m_StateMove == MOVE_RIGHT_PLUS) {
						m_Player->SetFrame(Vector2(5, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(5, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_RIGHT_PLUS;
					}
					else {
						m_Player->SetFrame(Vector2(4, m_Player->GetFrame().y));
						m_Player->GetSurfBoard()->SetFrame(Vector2(4, m_Player->GetSurfBoard()->GetFrame().y));
						m_StateMove = MOVE_RIGHT;
					}
					if (m_StatePlayer == PLAYER_DIE)
						m_StatePlayer = PLAYER_NORMAL;
				}
			}
			break;
		case VK_SHIFT:
			if (m_StateMove != FREEZE && m_StatePlayer != PLAYER_JUMP && m_Player->GetEnergy() > 0)
			{
				Application::GetInstance()->m_soloudEffects.play(m_plJumpMusic);
				m_Player->GetShadow()->SetSize(48, 48);
				m_StatePlayer = PLAYER_JUMP;
				m_StateMove = MOVE_FORWARD;
				m_ListEnergy.at(m_Player->GetEnergy() - 1)->SetFrame(Vector2(0, 2));
				m_Player->SetEnergy(m_Player->GetEnergy() - 1);
			}
			break;
		case VK_SPACE:
			Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
			if (m_SttGamePlay == GAME_RUNNING)
				Pause();
			else if (m_SttGamePlay == GAME_PAUSE)
				Resume();
			else if (m_SttGamePlay == GAME_OVER)
				ReStart();
			break;
		case VK_ESCAPE:
			Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
			if (m_SttGamePlay != GAME_OPTION)
			{
				GameOption();
			}
			else if (m_SttGamePlay == GAME_OPTION)
			{
				Resume();
			}
			break;
		default:
			break;
		}

	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	if (bIsPressed)
	{
		m_buttonOption->HandleTouchEvents(x, y, bIsPressed);
		if (m_buttonOption->IsHandle()) Application::GetInstance()->m_soloudEffects.play(m_ButtonMusic);
		if(m_SttGamePlay == GAME_OPTION)
			for (auto it : m_listButton)
			{
				(it)->HandleTouchEvents(x, y, bIsPressed);
				if ((it)->IsHandle()) break;
			}
	}
}

void GSPlay::Update(float deltaTime)
{
	if (!m_isPause && deltaTime > 0)
	{
		// sinh object moi
		if (m_TimeBornObj > 1) {
			RandomSprite();
			m_TimeBornObj -= 1;
		}
		m_TimeBornObj += m_ArrObject.back()->GetSpeed() / 1.5 * deltaTime;

		//sinh item
		if (m_TimeBornItem > 1) {
			RandomItem();
			m_TimeBornItem -= 1;
		}
		m_TimeBornItem += m_ArrObject.back()->GetSpeed() / 30 * deltaTime;

		//xu ly va cham
		this->DoCollision();

		//tang speed khi dat den nguong diem
		if (m_score > 100)
		{
			m_SpeedMove = 7;
		}
		else if (m_score > 300)
		{
			m_SpeedMove = 8;
		}
		else if (m_score > 600)
		{
			m_SpeedMove = 9;
		}
		else if (m_score > 1000)
		{
			m_SpeedMove = 10;
		}
		else if (m_score > 1500)
		{
			m_SpeedMove = 11;
		}

		//update
		m_Player->Update(deltaTime, m_StatePlayer);
		m_Player->GetSurfBoard()->Animation(deltaTime);
		for (auto& obj : m_ArrWater) {
			if ((GLint) m_score % 100 == 0)
				obj->SetMaxSpeed(m_SpeedMove);
			obj->Update(deltaTime, m_StateMove, m_StateSprite);
		}
		for (auto& obj : m_ArrObject) {
			if ((GLint) m_score % 100 == 0)
				obj->SetMaxSpeed(m_SpeedMove);
			obj->Update(deltaTime, m_StateMove, m_StateSprite);
			obj->Animation(deltaTime);
			if (obj->GetEffects() != nullptr)
			{
				obj->GetEffects()->Update(deltaTime, m_StateMove, m_StateSprite);
				obj->GetEffects()->Animation(deltaTime);
			}
		}

		//xet trang thai di chuyen neu ma khong dung yen thi set speed cua vat thanh binh thuong
		if (m_StateMove != FREEZE && m_StateSprite != SPEEDUP)
		{
			m_StateSprite = NORMAL;
		}

		//xoa object da di ra ngoai man hinh
		for (auto& obj : m_ArrObject) {
			if (obj->Get2DPosition().y < -(GLfloat)screenHeight / 4)
			{
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
				break;
			}
		}

		//update score
		m_score += m_ArrObject.back()->GetSpeed() * deltaTime;
		m_Text_score->setText(std::to_string((GLint) m_score) + "m");

		//player jump
		if (m_StatePlayer == PLAYER_JUMP)
		{
			if (m_Player->GetTimeJump() < 0)
			{
				m_Player->SetFrame(Vector2(3, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(3, m_Player->GetSurfBoard()->GetFrame().y));
				m_StatePlayer = PLAYER_NORMAL;
			}
		}
	}
	
	for (auto& obj : m_listButton)
	{
		obj->Update(deltaTime);
	}
}

void GSPlay::Draw()
{
	for (auto& obj : m_ArrWater) {
		obj->Draw();
	}

	for (auto& obj : m_ArrObject) {
		obj->Draw();
		if (obj->GetEffects() != nullptr)
		{
			obj->GetEffects()->Draw();
		}
	}

	if (m_StatePlayer == PLAYER_JUMP)
	{
		m_Player->GetShadow()->Draw();
	}
	m_Player->GetSurfBoard()->Draw();
	m_Player->Draw();

	if (m_SttGamePlay == GAME_PAUSE || m_SttGamePlay == GAME_OVER)
	{
		m_BackgroundOption->Draw();
		m_TextSttGame->Draw();
		m_DesciptSttGame->Draw();
	}

	m_buttonOption->Draw();

	if (m_SttGamePlay == GAME_OPTION)
	{
		m_BackgroundOption->Draw();
		for (auto& obj : m_listButton)
		{
			obj->Draw();
		}
	}

	m_Text_score->Draw();
	for (auto& obj : m_ListHeart) {
		obj->Draw();
	}
	for (auto& obj : m_ListEnergy) {
		obj->Draw();
	}
}

void GSPlay::SetNewPostionForBullet()
{
}

void GSPlay::DoCollision()
{
	for (auto& obj : m_ArrObject)
	{
		if (m_Player->CheckCollision(obj) && !m_Player->IsProtected())
		{
			if (obj->GetName() == "slowdown")
			{
				Application::GetInstance()->m_soloudEffects.play(m_SlowdownMusic);
				m_StateSprite = SLOWDOWN;
				if (m_Player->Get2DPosition().y > obj->Get2DPosition().y)
				{
					m_StateSprite = NORMAL;
					m_SlowdownMusic.stop();
				}
			}
			else if (obj->GetName() == "whirlpool")
			{
				Application::GetInstance()->m_soloudEffects.play(m_colWhirlPoolMusic);
				obj->SetName("whirlpool_collided");
				switch (m_StateMove)
				{
				case MOVE_LEFT:
					m_Player->SetFrame(Vector2(4, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(4, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = MOVE_RIGHT;
					break;
				case MOVE_RIGHT:
					m_Player->SetFrame(Vector2(2, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(2, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = MOVE_LEFT;
					break;
				case MOVE_FORWARD:
					m_Player->SetFrame(Vector2(2, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(2, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = MOVE_LEFT;
					break;
				case MOVE_LEFT_PLUS:
					m_Player->SetFrame(Vector2(4, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(4, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = MOVE_RIGHT;
					break;
				case MOVE_RIGHT_PLUS:
					m_Player->SetFrame(Vector2(2, m_Player->GetFrame().y));
					m_Player->GetSurfBoard()->SetFrame(Vector2(2, m_Player->GetSurfBoard()->GetFrame().y));
					m_StateMove = MOVE_LEFT;
					break;
				case FREEZE:
					break;
				default:
					break;
				}
			}
			else if (obj->GetName() == "harmful")
			{
				obj->SetName("harmful_collided");
				if (m_StatePlayer == PLAYER_NORMAL)
					Application::GetInstance()->m_soloudEffects.play(m_CollisionsMusic);
				m_StateMove = FREEZE;
				m_StatePlayer = PLAYER_DIE;
				m_StateSprite = SPEEDUP;
				if (m_Player->GetHeart() > 0 && m_Player->GetHeart() <= 3)
					m_ListHeart.at(m_Player->GetHeart() - 1)->SetFrame(Vector2(0, 3));
			}
			else if (obj->GetName() == "energy" && m_Player->GetEnergy() < 3)
			{
				Application::GetInstance()->m_soloudEffects.play(m_itemMusic);
				m_Player->SetEnergy(m_Player->GetEnergy() + 1);
				m_ListEnergy.at(m_Player->GetEnergy() - 1)->SetFrame(Vector2(1, 2));
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
			}
			else if (obj->GetName() == "heart" && m_Player->GetHeart() < 3)
			{
				Application::GetInstance()->m_soloudEffects.play(m_itemMusic);
				m_Player->SetHeart(m_Player->GetHeart() + 1);
				m_ListHeart.at(m_Player->GetHeart() - 1)->SetFrame(Vector2(1, 3));
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
			}

			break;
		}
		else if (m_Player->CheckCollision(obj))
		{
			if (obj->GetName() == "energy" && m_Player->GetEnergy() < 3)
			{
				Application::GetInstance()->m_soloudEffects.play(m_itemMusic);
				m_Player->SetEnergy(m_Player->GetEnergy() + 1);
				m_ListEnergy.at(m_Player->GetEnergy() - 1)->SetFrame(Vector2(1, 2));
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
			}
			else if (obj->GetName() == "heart" && m_Player->GetHeart() < 3)
			{
				Application::GetInstance()->m_soloudEffects.play(m_itemMusic);
				m_Player->SetHeart(m_Player->GetHeart() + 1);
				m_ListHeart.at(m_Player->GetHeart() - 1)->SetFrame(Vector2(1, 3));
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
			}
		}
	}
	if (m_Player->GetHeart() == 0)
	{
		GameOver();
	}
}

void GSPlay::RandomSprite()
{
	//printf("%d", m_ArrObject.size());
	std::vector<DynamicSpriteType> DynamicSpriteType;
	DynamicSpriteType.push_back(OBJECT64);
	DynamicSpriteType.push_back(OBJECT32);
	DynamicSpriteType.push_back(SANDBAR256);
	DynamicSpriteType.push_back(AMBIENT64);
	DynamicSpriteType.push_back(SLOWDOWN64);
	DynamicSpriteType.push_back(SLOWDOWN192);
	DynamicSpriteType.push_back(WHIRLPOOL);

	int IndexType = rand() % DynamicSpriteType.size();

	auto shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("objects64");
	std::shared_ptr<DynamicSprite> effect;
	std::shared_ptr<DynamicSprite> object;

	//set vi tri
	GLint XPos = (rand() % (2*screenWidth)) - screenWidth/2;
	GLint YPos = rand() % (screenHeight / 2) + screenHeight;

	switch (DynamicSpriteType.at(IndexType))
	{
	case OBJECT64:
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(64, 64);
		object->SetNumSprite(30);
		object->SetFrame(Vector2(rand() % 30, 0));
		object->Set2DPosition((GLfloat) XPos, (GLfloat) YPos);
		object->SetName("harmful");

		////add effect song nuoc
		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition(object->Get2DPosition());
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		effect->Init();
		object->SetEffects(effect);
		object->Init();
		break;
	case OBJECT32:
		texture = ResourceManagers::GetInstance()->GetTexture("objects32");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(32, 32);
		object->SetNumSprite(20);
		object->SetFrame(Vector2(rand() % 8, 0));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->SetName("harmful");

		//add effect song nuoc
		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition((GLfloat)XPos, (GLfloat)YPos - 15);
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		effect->Init();
		object->SetEffects(effect);
		object->Init();
		break;
	case SANDBAR256:
		texture = ResourceManagers::GetInstance()->GetTexture("sandbar256");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(256, 128);
		object->SetNumSprite(4);
		object->SetFrame(Vector2(rand() % 4, 0));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->Init();
		object->SetName("harmful");
		break;
	case AMBIENT64:
		texture = ResourceManagers::GetInstance()->GetTexture("ambient64");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(64, 64);
		object->SetNumSprite(4);
		object->SetNumFrame(6);
		object->SetFrame(Vector2(rand() % object->GetNumSprite(), object->GetNumFrame() - 1));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->SetName("harmful");

		//add effect song nuoc
		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		effect->Init();
		object->SetEffects(effect);
		object->Init();
		break;
	case SLOWDOWN64:
		texture = ResourceManagers::GetInstance()->GetTexture("slowdown64");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(64, 64);
		object->SetNumSprite(9);
		object->SetNumFrame(3);
		object->SetFrame(Vector2(rand() % object->GetNumSprite(), object->GetNumFrame() - 1));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->SetName("slowdown");
		object->Init();
		break;
	case SLOWDOWN192:
		texture = ResourceManagers::GetInstance()->GetTexture("slowdown64");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(192, 64);
		object->SetNumSprite(3);
		object->SetNumFrame(3);
		object->SetFrame(Vector2(rand() % object->GetNumSprite(), object->GetNumFrame() - 1));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->SetName("slowdown");
		object->Init();
		break;
	case WHIRLPOOL:
		texture = ResourceManagers::GetInstance()->GetTexture("effects128");
		object = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		object->SetSize(128, 128);
		object->SetNumSprite(6);
		object->SetNumFrame(3);
		object->SetFrame(Vector2(rand() % (object->GetNumSprite() - 2) , object->GetNumFrame() - 1));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
		object->SetName("whirlpool");
		object->Init();
		break;
	default:
		break;
	}
	int check = 0;
	for (auto& obj : m_ArrObject) {
		if (object->CheckCollision(obj))
		{
			check = 1;
			break;
		}
	}
	if (check == 0)
	{
		//set speed bang speed hien tai
		object->SetSpeed(m_ArrObject.back()->GetSpeed());
		if(object->GetEffects() != nullptr)
			object->GetEffects()->SetSpeed(m_ArrObject.back()->GetSpeed());
		m_ArrObject.push_back(object);
	}
	DynamicSpriteType.clear();
}

void GSPlay::RandomItem()
{
	std::vector<DynamicSpriteType> itemType;
	itemType.push_back(ENERGY);
	itemType.push_back(HEART);


	int IndexType = rand() % itemType.size();

	auto shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("interact64");
	std::shared_ptr<DynamicSprite> effect;
	std::shared_ptr<DynamicSprite> item;

	//set vi tri
	GLint XPos = (rand() % (2 * screenWidth)) - screenWidth / 2;
	GLint YPos = rand() % (screenHeight / 2) + screenHeight;

	switch (itemType.at(IndexType))
	{
	case ENERGY:
		texture = ResourceManagers::GetInstance()->GetTexture("interact64");
		item = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		item->SetName("energy");
		item->SetSize(64, 64);
		item->Set2DPosition(XPos, YPos);
		item->SetNumFrame(4);
		item->SetNumSprite(8);
		item->SetFrame(Vector2(2, 0));

		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition(item->Get2DPosition());
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		item->SetEffects(effect);
		break;
	case HEART:
		texture = ResourceManagers::GetInstance()->GetTexture("interact64");
		item = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		item->SetName("heart");
		item->SetSize(64, 64);
		item->Set2DPosition(XPos, YPos);
		item->SetNumFrame(4);
		item->SetNumSprite(8);
		item->SetFrame(Vector2(3, 0));

		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
		effect->SetSize(96, 96);
		effect->Set2DPosition(item->Get2DPosition());
		effect->SetNumFrame(3);
		effect->SetFrame(Vector2(0, 2));
		item->SetEffects(effect);
		break;
	default:
		break;
	}

	int check = 0;
	for (auto& obj : m_ArrObject) {
		if (item->CheckCollision(obj))
		{
			check = 1;
			break;
		}
	}
	if (check == 0)
	{
		//set speed bang speed hien tai
		item->SetSpeed(m_ArrObject.back()->GetSpeed());
		if (item->GetEffects() != nullptr)
			item->GetEffects()->SetSpeed(m_ArrObject.back()->GetSpeed());
		m_ArrObject.push_back(item);
	}
	itemType.clear();
}

