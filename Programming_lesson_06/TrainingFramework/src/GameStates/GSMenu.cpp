#include "GSMenu.h"
#include "GSPlay.h"
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

	//list player
	model = ResourceManagers::GetInstance()->GetModel("Player");
	texture = ResourceManagers::GetInstance()->GetTexture("player64");
	shader = ResourceManagers::GetInstance()->GetShader("ObjectShader");
	for (int i = 0; i < 9; i++) 
	{
		m_ListPlayer.push_back(std::make_shared<Player>(model, shader, texture));
	}
	i = 0;
	for(auto &obj : m_ListPlayer)
	{
		obj->SetSize(64, 64);
		obj->Set2DPosition(i * 96 + (GLfloat)screenWidth / 2, 200);
		obj->SetNumEffects(9);
		obj->SetNumSprites(13);
		obj->SetShift((GLfloat)3 / obj->GetNumSprites(), (GLfloat)i / obj->GetNumEffects());
		obj->Init();
		i++;
	}
	//surfboard
	model = ResourceManagers::GetInstance()->GetModel("SurfBoard");
	texture = ResourceManagers::GetInstance()->GetTexture("SurfBoard64");
	for (int i = 0; i < 9; i++)
	{
		m_ListSurfBoard.push_back(std::make_shared<SurfBoard>(model, shader, texture));
	}
	i = 0;
	for (auto& obj : m_ListSurfBoard)
	{
		obj->SetSize(64, 64);
		obj->Set2DPosition(i * 96 + (GLfloat)screenWidth / 2, 200);
		obj->SetNumEffects(3);
		obj->SetNumSprites(13);
		obj->SetShift((GLfloat) 3 / obj->GetNumSprites(), (GLfloat)2 / obj->GetNumEffects());
		obj->Init();
		i++;
	}

	//auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	//auto texture = ResourceManagers::GetInstance()->GetTexture("bg_main_menu");

	////BackGround
	//auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	//m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	//m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	//m_BackGround->SetSize(screenWidth, screenHeight);

	////play button
	//texture = ResourceManagers::GetInstance()->GetTexture("button_play");
	//std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	//button->Set2DPosition(screenWidth / 2, 200);
	//button->SetSize(200, 50);
	//button->SetOnClick([]() {
	//	GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	//	});
	//m_listButton.push_back(button);

	////exit button
	//texture = ResourceManagers::GetInstance()->GetTexture("button_quit");
	//button = std::make_shared<GameButton>(model, shader, texture);
	//button->Set2DPosition(screenWidth / 2, 300);
	//button->SetSize(200, 50);
	//button->SetOnClick([]() {
	//	exit(0);
	//	});
	//m_listButton.push_back(button);

	//arow left
	model = ResourceManagers::GetInstance()->GetModel("Arrow");
	texture = ResourceManagers::GetInstance()->GetTexture("arrow");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition((GLfloat)screenWidth / 2 - 48, 200);
	button->SetSize(32, 32);
	button->SetOnClick([]() {

		});
	m_listButton.push_back(button);

	//arow right
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition((GLfloat)screenWidth / 2 + 48, 200);
	button->SetSize(32, 32); 
	button->SetShift((GLfloat)1/2, 0);
	button->SetOnClick([]() {

		});
	m_listButton.push_back(button);

	////text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared< Text>(shader, font, "LET'S SURF", TEXT_COLOR::BLACK, 2.0);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 140, 120));
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
					DefautPlayer = (GLint) (obj->GetXYShiftPosition().y * obj->GetNumEffects());
					break;
				}
			}
			//printf("%d", DefautPlayer);
			GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
			break;
		case 'D':
			if (m_ListPlayer.back()->Get2DPosition().x > GLfloat(screenWidth) / 2)
			{
				ChangePlayerRight();
			}
			break;
		case 'A':
			if (m_ListPlayer.front()->Get2DPosition().x < GLfloat(screenWidth) / 2)
			{
				ChangePlayerLeft();
			}
			break;
		default:
			break;
		}
	}

}

void GSMenu::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	/*for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}*/
}

void GSMenu::Update(float deltaTime)
{
	for (auto& obj : m_ListSurfBoard)
	{
		if (obj->Get2DPosition().x == (GLfloat)screenWidth / 2)
		{
			obj->MenuSurfAnimation(deltaTime);
		}
		/*else 
			obj->SetShift((GLfloat)3 / obj->GetNumSprites(), obj->GetXYShiftPosition().y);*/
		obj->Animation(deltaTime);
	}
	for (auto& obj : m_ListPlayer)
	{
		if (obj->Get2DPosition().x == (GLfloat)screenWidth / 2)
		{
			obj->MenuPlayerAnimation(deltaTime);
		}
		/*else 
			obj->SetShift((GLfloat)3 / obj->GetNumSprites(), obj->GetXYShiftPosition().y);*/
	}
	//m_BackGround->Update(deltaTime);
	/*for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}*/
}

void GSMenu::Draw()
{
	for (auto& obj : m_ArrWater)
	{
		obj->Draw();
	}
	for (auto& obj : m_ListSurfBoard)
	{
		obj->Draw();
	}
	for (auto& obj : m_ListPlayer)
	{
		obj->Draw();
	}
	//m_BackGround->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_Text_gameName->Draw();
}

void GSMenu::ChangePlayerLeft()
{
	for (auto& obj : m_ListPlayer)
	{
		obj->Set2DPosition(obj->Get2DPosition().x + 96, obj->Get2DPosition().y);
	}
	for (auto& obj : m_ListSurfBoard)
	{
		obj->Set2DPosition(obj->Get2DPosition().x + 96, obj->Get2DPosition().y);
	}
}

void GSMenu::ChangePlayerRight()
{
	for (auto& obj : m_ListPlayer)
	{
		obj->Set2DPosition(obj->Get2DPosition().x - 96, obj->Get2DPosition().y);
	}
	for (auto& obj : m_ListSurfBoard)
	{
		obj->Set2DPosition(obj->Get2DPosition().x - 96, obj->Get2DPosition().y);
	}
}
