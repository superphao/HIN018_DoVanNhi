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

	m_StateMove = FREEZE;
	m_SttGamePlay = GAME_RUNNING;
	m_TimeBornObj = 0;
	m_score = 0;
	m_SpeedMove = 5;
	m_isPause = false;

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	//player
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	m_Player = std::make_shared<Player>(model, shader, texture);
	m_Player->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	m_Player->SetSize(64, 64);
	m_Player->SetNumFrame(9);
	m_Player->SetNumSprite(13);
	m_Player->SetFrame(Vector2(0, DefautPlayer));
	m_Player->Init();
	m_Player->SetName("player");

	//surfboard
	texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
	std::shared_ptr<Sprite2D> surfBoard = std::make_shared<Sprite2D>(model, shader, texture);
	surfBoard->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	surfBoard->SetSize(64, 64);
	surfBoard->SetNumFrame(3);
	surfBoard->SetNumSprite(13);
	surfBoard->SetFrame(Vector2(0, 2));
	surfBoard->Init();
	m_Player->SetSurfBoard(surfBoard);

	//Water
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
		//printf("%f, %f\n", obj->Get2DPosition().x, obj->Get2DPosition().y);
	}

	//object
	texture = ResourceManagers::GetInstance()->GetTexture("objects64");
	m_ArrObject.push_back(std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove));
	m_ArrObject.at(0)->SetSize(64, 64);
	m_ArrObject.at(0)->SetNumSprite(30);
	srand(time(NULL));
	m_ArrObject.at(0)->Set2DPosition((GLfloat)(rand() % 640), rand() % (screenHeight / 2) + m_Player->Get2DPosition().y);
	m_ArrObject.at(0)->Init();
	texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
	std::shared_ptr<DynamicSprite> effect = std::make_shared<DynamicSprite>(model, shader, texture, m_SpeedMove);
	effect->SetSize(96, 96);
	effect->Set2DPosition(m_ArrObject.back()->Get2DPosition());
	effect->SetNumFrame(3);
	effect->SetFrame(Vector2(0, 2));
	effect->Init();
	m_ArrObject.at(0)->SetEffects(effect);

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
		//GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Option);
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
		GameStateMachine::GetInstance()->ChangeState(STATE_Menu);
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

	//open file high score
	//open file highscore
	std::ifstream readFile("..\\Data\\Highscore\\Highscore.txt", std::ios::binary);
	if (!readFile.is_open())
	{
		LOGE("ERROR Highscore.txt \n");
		return;
	}
	else
	{
		LOGI("Load File:Highscore.txt\t\t");

		while (!readFile.eof())
		{
			readFile >> m_highscore;
		}
		readFile.close();
	}

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
	m_SttGamePlay = GAME_OVER;
	m_TextSttGame->setText("SURF AGAIN");
	m_DesciptSttGame->setText("SPACEBAR to surf again");
	m_isPause = true;
	if (m_score > m_highscore)
	{
		std::ofstream writeFile("..\\Data\\Highscore\\Highscore.txt", std::ios::binary);
		if (!writeFile.is_open())
		{
			LOGE("ERROR Highscore.txt \n");
			return;
		}
		else
		{
			LOGI("Load File:Highscore.txt\t\t");
			writeFile << (GLint) m_score;
			writeFile.close();
		}
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
			m_Player->SetFrame(Vector2(3, m_Player->GetFrame().y));
			m_Player->GetSurfBoard()->SetFrame(Vector2(3, m_Player->GetSurfBoard()->GetFrame().y));
			m_StateMove = MOVE_FORWARD;
			if(m_StatePlayer == PLAYER_DIE)
				m_StatePlayer = PLAYER_NORMAL;
			break;
		case 'W':
			m_Player->SetFrame(Vector2(0, m_Player->GetFrame().y));
			m_Player->GetSurfBoard()->SetFrame(Vector2(0, m_Player->GetSurfBoard()->GetFrame().y));
			m_StateMove = FREEZE;
			break;
		case 'A':
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
			break;
		case 'D':
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
			break;
		case VK_SPACE:
			if (m_SttGamePlay == GAME_RUNNING)
				Pause();
			else if (m_SttGamePlay == GAME_PAUSE)
				Resume();
			else if (m_SttGamePlay == GAME_OVER)
				ReStart();
			break;
		case VK_ESCAPE:
			if (m_SttGamePlay == GAME_RUNNING)
			{
				GameOption();
			}
			else if (m_SttGamePlay == GAME_OPTION)
			{
				Resume();
			}
			break;
		default:
			if (m_SttGamePlay == GAME_RUNNING)
				Pause();
			break;
		}

	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	if (bIsPressed)
	{
		m_buttonOption->HandleTouchEvents(x, y, bIsPressed);
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
		m_TimeBornObj += m_ArrObject.back()->GetSpeed() /2 * deltaTime;
		//m_TimeBornObj += deltaTime;

		//xu ly va cham
		this->DoCollision();

		//update
		m_Player->Update(deltaTime, m_StatePlayer);
		//m_Player->GetSurfBoard()->Update(deltaTime)
		m_Player->GetSurfBoard()->Animation(deltaTime);
		for (auto& obj : m_ArrWater) {
			obj->Update(deltaTime, m_StateMove, m_StateSprite);
		}
		for (auto& obj : m_ArrObject) {
			obj->Update(deltaTime, m_StateMove, m_StateSprite);
			obj->Animation(deltaTime);
			if (obj->GetEffects() != nullptr)
			{
				obj->GetEffects()->Update(deltaTime, m_StateMove, m_StateSprite);
				obj->GetEffects()->Animation(deltaTime);
			}
		}

		//xet trang thai di chuyen neu ma khong dung yen thi set speed cua vat thanh binh thuong
		if (m_StateMove != FREEZE)
		{
			m_StateSprite = NORMAL;
		}

		//xoa object da di ra ngoai man hinh
		for (auto& obj : m_ArrObject) {
			if (obj->Get2DPosition().y < -(GLfloat)screenHeight / 4)
			{
				obj.swap(m_ArrObject.back());
				m_ArrObject.pop_back();
				//printf("%d", m_ArrObject.size());
				break;
			}
		}
		m_score += m_ArrObject.back()->GetSpeed() * deltaTime;
		m_Text_score->setText(std::to_string((GLint) m_score) + "m");
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
	for (auto& obj : m_ListHeart) {
		obj->Draw();
	}
	for (auto& obj : m_ListEnergy) {
		obj->Draw();
	}

	m_Player->GetSurfBoard()->Draw();
	m_Player->Draw();
	for (auto& obj : m_ArrObject) {
		obj->Draw();
		if (obj->GetEffects() != nullptr)
		{
			obj->GetEffects()->Draw();
		}
	}
	//m_BackGround->Draw();
	m_Text_score->Draw();
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
}

void GSPlay::SetNewPostionForBullet()
{
}

void GSPlay::DoCollision()
{
	for (auto& obj : m_ArrObject)
		if (m_Player->CheckCollision(obj) && !m_Player->IsProtected())
		{
			if (obj->GetName() == "slowdown")
			{
				//if(m_StateSprite != SLOWDOWN)
					m_StateSprite = SLOWDOWN;
					if (m_Player->Get2DPosition().y > obj->Get2DPosition().y)
					{
						m_StateSprite = NORMAL;
					}
			}
			else if (obj->GetName() == "whirlpool")
			{
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
			else if(obj->GetName() == "harmful")
			{
				m_StateMove = FREEZE;
				m_StatePlayer = PLAYER_DIE;
				if (m_Player->GetHeart() >= 0 && m_Player->GetHeart() < 3)
					m_ListHeart.at(m_Player->GetHeart())->SetFrame(Vector2(0, 3));
			}
			break;
		}
	if (m_Player->GetHeart() == 0)
	{
		GameOver();
	}
	//printf("%f", m_ArrObject.at(0)->GetSpeed());
}

void GSPlay::RandomSprite()
{
	//printf("%d", m_ArrObject.size());
	//srand(time(NULL));
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
		m_ArrObject.push_back(object);
	}
	DynamicSpriteType.clear();
}

