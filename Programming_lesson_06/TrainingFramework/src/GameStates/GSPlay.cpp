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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_StateSprite = FREEZE;
	m_SttGamePlay = GAME_RUNNING;
	m_TimeBornObj = 0;
	m_TextScore = 0;
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
		m_ArrWater.push_back(std::make_shared<DynamicSprite>(model, shader, texture));
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
	m_ArrObject.push_back(std::make_shared<DynamicSprite>(model, shader, texture));
	m_ArrObject.at(0)->SetSize(64, 64);
	m_ArrObject.at(0)->SetNumSprite(30);
	srand(time(NULL));
	m_ArrObject.at(0)->Set2DPosition((GLfloat)(rand() % 640), rand() % (screenHeight / 2) + m_Player->Get2DPosition().y);
	m_ArrObject.at(0)->Init();
	texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
	std::shared_ptr<DynamicSprite> effect = std::make_shared<DynamicSprite>(model, shader, texture);
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
		std::shared_ptr<Sprite2D> m_Heart = std::make_shared<Sprite2D>(model, shader, texture);
		m_Heart->SetSize(24, 24);
		m_Heart->Set2DPosition((GLfloat)screenWidth / 2 - 170 + i * 24, 24);
		m_Heart->SetNumFrame(4);
		m_Heart->SetNumSprite(2);
		m_Heart->SetFrame(Vector2(1, 3));
		m_Heart->Init();
		m_ListHeart.push_back(m_Heart);
	}

	//score
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: " , TEXT_COLOR::BLACK, 0.7);
	m_score->Set2DPosition(Vector2((GLfloat) screenWidth/2 - 50, 25));

	//text state game play
	m_TextSttGame = std::make_shared< Text>(shader, font, "SURF PAUSED", TEXT_COLOR::BLACK, 1.5);
	m_TextSttGame->Set2DPosition(Vector2((GLfloat)screenWidth / 2 - 110, 100));

	//text decription state game
	m_DesciptSttGame = std::make_shared< Text>(shader, font, "SPACEBAR to resume surfing", TEXT_COLOR::BLACK, 0.8);
	m_DesciptSttGame->Set2DPosition(Vector2((GLfloat)screenWidth / 2 - 120, 140));
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
}

void GSPlay::ReStart()
{
	GameStateMachine::GetInstance()->PopState();
	GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
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
			m_StateSprite = MOVE_FORWARD;
			if(m_StatePlayer == PLAYER_DIE)
				m_StatePlayer = PLAYER_NORMAL;
			break;
		case 'W':
			m_Player->SetFrame(Vector2(0, m_Player->GetFrame().y));
			m_Player->GetSurfBoard()->SetFrame(Vector2(0, m_Player->GetSurfBoard()->GetFrame().y));
			m_StateSprite = FREEZE;
			break;
		case 'A':
			if (m_StateSprite == MOVE_LEFT) {
				m_Player->SetFrame(Vector2(1, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(1, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_LEFT_PLUS;
			}
			else if (m_StateSprite == MOVE_LEFT_PLUS) {
				m_Player->SetFrame(Vector2(1, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(1, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_LEFT_PLUS;
			}
			else {
				m_Player->SetFrame(Vector2(2, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(2, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_LEFT;
			}
			if (m_StatePlayer == PLAYER_DIE)
				m_StatePlayer = PLAYER_NORMAL;
			break;
		case 'D':
			if (m_StateSprite == MOVE_RIGHT) {
				m_Player->SetFrame(Vector2(5, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(5, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_RIGHT_PLUS;
			}
			else if (m_StateSprite == MOVE_RIGHT_PLUS) {
				m_Player->SetFrame(Vector2(5, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(5, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_RIGHT_PLUS;
			}
			else {
				m_Player->SetFrame(Vector2(4, m_Player->GetFrame().y));
				m_Player->GetSurfBoard()->SetFrame(Vector2(4, m_Player->GetSurfBoard()->GetFrame().y));
				m_StateSprite = MOVE_RIGHT;
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
		default:
			if (m_SttGamePlay == GAME_RUNNING)
				Pause();
			break;
		}

	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSPlay::Update(float deltaTime)
{
	if (!m_isPause)
	{
		// sinh object moi
		if (m_TimeBornObj > 1) {
			RandomSprite();
			m_TimeBornObj -= 1;
		}
		//count += m_ArrObject.back()->GetSpeed() * deltaTime;
		m_TimeBornObj += deltaTime;

		//xu ly va cham
		this->DoCollision();

		//update
		m_Player->Update(deltaTime, m_StatePlayer);
		//m_Player->GetSurfBoard()->Update(deltaTime)
		m_Player->GetSurfBoard()->Animation(deltaTime);
		for (auto& obj : m_ArrWater) {
			obj->Update(deltaTime, m_StateSprite);
		}
		for (auto& obj : m_ArrObject) {
			obj->Update(deltaTime, m_StateSprite);
			if (obj->GetEffects() != nullptr)
			{
				obj->GetEffects()->Update(deltaTime, m_StateSprite);
				obj->GetEffects()->Animation(deltaTime);
			}
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
		m_TextScore += m_ArrObject.back()->GetSpeed() * deltaTime;
		m_score->setText("Score: " + std::to_string((GLint) m_TextScore) + "m");
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
	//m_Player->GetSurfBoard()->Draw();
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
	m_score->Draw();
	if (m_SttGamePlay == GAME_PAUSE || m_SttGamePlay == GAME_OVER)
	{
		m_TextSttGame->Draw();
		m_DesciptSttGame->Draw();
	}
}

void GSPlay::SetNewPostionForBullet()
{
}

void GSPlay::DoCollision()
{
	for (auto& obj : m_ArrObject)
		if (m_Player->CheckCollision(obj) && !m_Player->CheckProtected())
		{
			m_StateSprite = FREEZE;
			m_StatePlayer = PLAYER_DIE;
			//printf("%d", m_Player->GetHeart());
			if(m_Player->GetHeart() >= 0 && m_Player->GetHeart() <3)
				m_ListHeart.at(m_Player->GetHeart())->SetFrame(Vector2(0, 3));
			break;
		}
	if (m_Player->GetHeart() == 0)
	{
		GameOver();
	}
}

void GSPlay::RandomSprite()
{
	//printf("%d", m_ArrObject.size());
	srand(time(NULL));
	std::vector<DynamicSpriteType> DynamicSpriteType;
	DynamicSpriteType.push_back(OBJECT64);
	DynamicSpriteType.push_back(OBJECT32);
	DynamicSpriteType.push_back(SANDBAR256);
	int IndexType = rand() % DynamicSpriteType.size();

	auto shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("objects64");
	std::shared_ptr<DynamicSprite> effect;
	std::shared_ptr<DynamicSprite> object;

	//set vi tri
	GLint XPos = rand() % 640;
	GLint YPos = rand() % (screenHeight / 2) + screenHeight;

	switch (DynamicSpriteType.at(IndexType))
	{
	case OBJECT64:
		object = std::make_shared<DynamicSprite>(model, shader, texture);
		object->SetSize(64, 64);
		object->SetNumSprite(30);
		object->SetFrame(Vector2(rand() % 30, 0));
		object->Set2DPosition((GLfloat) XPos, (GLfloat) YPos);

		////add effect song nuoc
		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture);
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
		object = std::make_shared<DynamicSprite>(model, shader, texture);
		object->SetSize(32, 32);
		object->SetNumSprite(20);
		object->SetFrame(Vector2(rand() % 8, 0));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);

		//add effect song nuoc
		texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
		effect = std::make_shared<DynamicSprite>(model, shader, texture);
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
		object = std::make_shared<DynamicSprite>(model, shader, texture);
		object->SetSize(256, 128);
		object->SetNumSprite(4);
		object->SetFrame(Vector2(rand() % 4, 0));
		object->Set2DPosition((GLfloat)XPos, (GLfloat)YPos);
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

