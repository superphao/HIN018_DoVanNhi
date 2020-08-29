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
GLfloat count = 0;

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

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	//player
	model = ResourceManagers::GetInstance()->GetModel("Player");
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	m_Player = std::make_shared<Sprite2D>(model, shader, texture);
	m_Player->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	m_Player->SetSize(64, 64);
	m_Player->SetNumEffects(9);
	m_Player->SetNumSprites(13);
	m_Player->Init();
	m_Player->SetShift(0, (GLfloat) DefautPlayer/m_Player->GetNumEffects());

	//surfboard
	model = ResourceManagers::GetInstance()->GetModel("SurfBoard");
	texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
	m_SurfBoard = std::make_shared<Sprite2D>(model, shader, texture);
	m_SurfBoard->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	m_SurfBoard->SetSize(64, 64);
	m_SurfBoard->SetNumEffects(3);
	m_SurfBoard->SetNumSprites(13);
	m_SurfBoard->SetShift(0, (GLfloat)2 / m_SurfBoard->GetNumEffects());
	m_SurfBoard->Init();

	//Water
	model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	texture = ResourceManagers::GetInstance()->GetTexture("water256");
	for (int i = 0; i < 16; ++i) {
		m_ArrWater.push_back(std::make_shared<Sprite2D>(model, shader, texture));
	}
	int i = 0, j = 0;
	for (auto& obj : m_ArrWater) {
		obj->SetSize((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2);
		obj->Set2DPosition(i * screenWidth / 2 - (GLfloat)screenWidth / 4, j * screenHeight / 2 - (GLfloat)screenHeight / 4);
		obj->Init();
		i++;
		if (i == 4) {
			i = 0;
			j++;
		}
		//printf("%f, %f\n", obj->Get2DPosition().x, obj->Get2DPosition().y);
	}

	//object
	model = ResourceManagers::GetInstance()->GetModel("Object64");
	texture = ResourceManagers::GetInstance()->GetTexture("objects64");
	m_ArrObject.push_back(std::make_shared<Enemy>(model, shader, texture));
	m_ArrObject.at(0)->SetSize(64, 64);
	srand(time(NULL));
	m_ArrObject.at(0)->Set2DPosition((GLfloat)(rand() % 640), rand() % (screenHeight / 2) + m_Player->Get2DPosition().y);
	m_ArrObject.at(0)->Init();

	model = ResourceManagers::GetInstance()->GetModel("EffectObj");
	texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
	std::shared_ptr<Sprite2D> effect = std::make_shared<Sprite2D>(model, shader, texture);
	effect->SetSize(96, 96);
	effect->Set2DPosition(m_ArrObject.back()->Get2DPosition());
	effect->SetNumEffects(3);
	effect->SetShift(0, (GLfloat)2 / effect->GetNumEffects());
	effect->SetRange(m_ArrObject.back()->GetRange());
	effect->Init();
	m_ArrObject.at(0)->SetEffects(effect);

	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	//auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	////BackGround
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	//m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	//m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	//m_BackGround->SetSize(screenWidth, screenHeight);


	////text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: " + std::to_string(count), TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed) {
		switch (key) {
		case 'S':
			m_Player->SetShift((GLfloat)3 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)3 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(0, 2));
			}
			for (auto& obj : m_ArrObject) {
				obj->SetRange(Vector2(0, 2));
				if(obj->GetEffects() != nullptr)
					obj->GetEffects()->SetRange(Vector2(0, 2));
			}
			break;
		case 'W':
			m_Player->SetShift(0, m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift(0, m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(0, 0));
			}
			for (auto& obj : m_ArrObject) {
				obj->SetRange(Vector2(0, 0));
				if (obj->GetEffects() != nullptr)
					obj->GetEffects()->SetRange(Vector2(0, 0));
			}
			break;
		case 'A':
			if (m_Player->GetXYShiftPosition().x == (GLfloat)2 / m_Player->GetNumSprites()) {
				m_Player->SetShift((GLfloat)1 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)1 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(-4, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(-4, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(-4, 2));
				}
			}
			else if (m_Player->GetXYShiftPosition().x == (GLfloat)1 / m_Player->GetNumSprites()) {
				m_Player->SetShift((GLfloat)1 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)1 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(-4, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(-4, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(-4, 2));
				}
			}
			else {
				m_Player->SetShift((GLfloat)2 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)2 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(-2, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(-2, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(-2, 2));
				}
			}
			break;
		case 'D':
			if (m_Player->GetXYShiftPosition().x == (GLfloat)4 / m_Player->GetNumSprites()) {
				m_Player->SetShift((GLfloat)5 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)5 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(4, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(4, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(4, 2));
				}
			}
			else if (m_Player->GetXYShiftPosition().x == (GLfloat)5 / m_Player->GetNumSprites()) {
				m_Player->SetShift((GLfloat)5 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)5 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(4, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(4, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(4, 2));
				}
			}
			else {
				m_Player->SetShift((GLfloat)4 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
				m_SurfBoard->SetShift((GLfloat)4 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
				for (auto& obj : m_ArrWater) {
					obj->SetRange(Vector2(2, 2));
				}
				for (auto& obj : m_ArrObject) {
					obj->SetRange(Vector2(2, 2));
					if (obj->GetEffects() != nullptr)
						obj->GetEffects()->SetRange(Vector2(2, 2));
				}
			}
			break;
		}
	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSPlay::Update(float deltaTime)
{
	// sinh object moi
	if (count > 1) {
		RandomSprite();
		count = 0;
	}
	count += deltaTime/2;

	//xu ly va cham
	this->DoCollision();

	//update
	//m_Player->Update(deltaTime);
	//m_SurfBoard->Update(deltaTime)
	m_SurfBoard->Animation(deltaTime);
	for (auto& obj : m_ArrWater) {
		obj->Update(deltaTime);
	}
	for (auto& obj : m_ArrObject) {
		obj->Update(deltaTime);
		if (obj->GetEffects() != nullptr)
		{
			obj->GetEffects()->Update(deltaTime);
			obj->GetEffects()->Animation(deltaTime);
		}
	}

	//xoa object da di ra ngoai man hinh
	for (auto& obj : m_ArrObject) {
		if (obj->Get2DPosition().y < - (GLfloat)screenHeight / 4)
		{
			obj.swap(m_ArrObject.back());
			m_ArrObject.pop_back();
			//printf("%d", m_ArrObject.size());
			break;
		}
	}
	m_score->setText("Score: " + std::to_string(count));
}

void GSPlay::Draw()
{
	for (auto& obj : m_ArrWater) {
		obj->Draw();
	}
	m_SurfBoard->Draw();
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
}

void GSPlay::SetNewPostionForBullet()
{
}

void GSPlay::DoCollision()
{
	for (auto& obj : m_ArrObject)
		if (m_Player->CheckCollision(obj))
		{
			for (auto& obj : m_ArrObject)
			{
				obj->SetRange(Vector2(0, 0));
				if(obj->GetEffects() != nullptr)
					obj->GetEffects()->SetRange(Vector2(0, 0));
			}
			for (auto& obj : m_ArrWater) 
			{	
				obj->SetRange(Vector2(0, 0));
			}
			m_Player->SetShift((GLfloat)6 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)6 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
		}

}

void GSPlay::RandomSprite()
{
	auto shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	auto model = ResourceManagers::GetInstance()->GetModel("Object64");
	auto texture = ResourceManagers::GetInstance()->GetTexture("objects64");
	m_ArrObject.push_back(std::make_shared<Enemy>(model, shader, texture));
	m_ArrObject.back()->SetSize(64, 64);
	srand(time(NULL));
	m_ArrObject.back()->SetShift((GLfloat)(rand() % 30) / 30, 0);
	//m_ArrObject.back()->SetShift((GLfloat) 3/30, 0);
	m_ArrObject.back()->Set2DPosition((GLfloat)(rand() % 640), rand() % (screenHeight / 2) + (screenHeight));
	m_ArrObject.back()->SetRange(m_ArrObject.at(0)->GetRange());

	//add effect
	model = ResourceManagers::GetInstance()->GetModel("EffectObj");
	texture = ResourceManagers::GetInstance()->GetTexture("ripple96");
	std::shared_ptr<Sprite2D> effect = std::make_shared<Sprite2D>(model, shader, texture);
	effect->SetSize(96, 96);
	effect->Set2DPosition(m_ArrObject.back()->Get2DPosition());
	effect->SetNumEffects(3);
	effect->SetShift(0, (GLfloat)2 / effect->GetNumEffects());
	effect->SetRange(m_ArrObject.back()->GetRange());
	effect->Init();
	m_ArrObject.back()->SetEffects(effect);
	m_ArrObject.back()->Init();

}

