#include "GSIntro.h"


extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine


GSIntro::GSIntro()
{
	m_time = 0;
}


GSIntro::~GSIntro()
{
}


void GSIntro::Init()
{
	glClearColor(56.0f / 255.0f, 194.0f / 255.0f, 238.0f / 255.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// water
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("water256");
	for (int i = 0; i < 16; ++i) {
		m_ArrWater.push_back(std::make_shared<DynamicSprite>(model, shader, texture, 7));
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

	//enemy
	texture = ResourceManagers::GetInstance()->GetTexture("enemy128");
	m_enemy = std::make_shared<DynamicSprite>(model, shader, texture, 7);
	m_enemy->SetName("energy");
	m_enemy->SetSize(128, 128);
	m_enemy->Set2DPosition(screenWidth / 2, 0);
	m_enemy->SetNumFrame(6);
	m_enemy->SetNumSprite(2);
	m_enemy->SetFrame(Vector2(0, 5));
	m_enemy->Init();

	//player
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	shader = ResourceManagers::GetInstance()->GetShader("ShadowShader");
	m_Player = std::make_shared<Player>(model, shader, texture);
	m_Player->Set2DPosition((GLfloat)screenWidth / 2, 64);
	m_Player->SetSize(64, 64);
	m_Player->SetNumFrame(9);
	m_Player->SetNumSprite(13);
	m_Player->SetFrame(Vector2(3, 1));

	//surfboard
	texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
	std::shared_ptr<Sprite2D> surfBoard = std::make_shared<Sprite2D>(model, shader, texture);
	surfBoard->Set2DPosition((GLfloat)screenWidth / 2, 64);
	surfBoard->SetSize(64, 64);
	surfBoard->SetNumFrame(3);
	surfBoard->SetNumSprite(13);
	surfBoard->SetFrame(Vector2(3, 2));
	m_Player->SetSurfBoard(surfBoard);

}

void GSIntro::Exit()
{
	
}


void GSIntro::Pause()
{
	
}

void GSIntro::Resume()
{
	
}


void GSIntro::HandleEvents()
{
	
}

void GSIntro::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSIntro::HandleTouchEvents(int x, int y, bool bIsPressed)
{

}

void GSIntro::Update(float deltaTime)
{
	m_enemy->Animation(deltaTime);
	if (m_Player->GetSurfBoard() != nullptr)
		m_Player->GetSurfBoard()->Animation(deltaTime);

	if (m_enemy->Get2DPosition().y < screenHeight / 2)
	{
		m_enemy->Set2DPosition(m_enemy->Get2DPosition().x, m_enemy->Get2DPosition().y + 480*deltaTime);
	}
	else
	{
		m_enemy->SetFrame(Vector2(1, m_enemy->GetFrame().y));
	}

	if (m_Player->Get2DPosition().y < screenHeight / 2 + 64)
	{
		m_Player->Set2DPosition(m_Player->Get2DPosition().x, m_Player->Get2DPosition().y + 480 * deltaTime);
		if (m_Player->GetSurfBoard() != nullptr)
			m_Player->GetSurfBoard()->Set2DPosition(m_Player->Get2DPosition());
	}
	else
	{
		m_Player->SetFrame(Vector2(7, m_Player->GetFrame().y));
		if (m_Player->GetSurfBoard() != nullptr)
			m_Player->GetSurfBoard()->SetFrame(Vector2(7, m_Player->GetSurfBoard()->GetFrame().y));
	}

	m_time += deltaTime;
	if (m_time > 2)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		m_time = 0;
	}
}

void GSIntro::Draw()
{

	for (auto& obj : m_ArrWater)
	{
		obj->Draw();
	}
	
	if (m_Player->GetSurfBoard() != nullptr)
	{
		m_Player->GetSurfBoard()->Draw();
	}
	m_Player->Draw();
	m_enemy->Draw();
}
