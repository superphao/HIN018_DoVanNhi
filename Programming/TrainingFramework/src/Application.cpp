#include "Application.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
extern GLint screenWidth;
extern GLint screenHeight;

Application::Application(void)
{
}


Application::~Application(void)
{
}


void Application::Init()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//blue
	glClearColor(56.0f/255.0f, 194.0f/255.0f, 238.0f/255.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shaders *m_Shaders;
	Texture *m_texture;
	Models *m_model;
	Camera *m_Camera; 

	Vector3 CameraPos(10, 40, 40);
	Vector3 TargetPos(0, 0, 0);
	float fFovY = 0.7f;
	m_Camera = new Camera();
	m_Camera->Init(CameraPos, TargetPos, fFovY, (GLfloat)screenWidth / screenHeight, 1.0f, 5000.0f, 1.0f);

	//player
	m_Shaders = new Shaders();
	m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");

	m_texture = new Texture();
	m_texture->Init("..\\Data\\Textures\\player64.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	m_model = new Models();
	m_model->Init("..\\Data\\Model\\Player.nfg", NFG);

	m_Player = new Sprite2D(m_model,m_Shaders,m_texture);
	m_Player->Set2DPosition((GLfloat) screenWidth/2, (GLfloat)(screenHeight / 2 - 100));
	m_Player->SetSize(64, 64);
	m_Player->SetNumEffects(9);
	m_Player->SetNumSprites(13);
	m_Player->Init();
	m_Player->SetShift(0 , (GLfloat)8 / m_Player->GetNumEffects());

	//surfboard
	m_Shaders = new Shaders();
	m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");

	m_texture = new Texture();
	m_texture->Init("..\\Data\\Textures\\SurfBoard64.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	m_model = new Models();
	m_model->Init("..\\Data\\Model\\SurfBoard.nfg", NFG);

	m_SurfBoard = new Sprite2D(m_model, m_Shaders, m_texture);
	m_SurfBoard->Set2DPosition((GLfloat)screenWidth / 2, (GLfloat)(screenHeight / 2 - 100));
	m_SurfBoard->SetSize(64, 64);
	m_SurfBoard->SetNumEffects(3);
	m_SurfBoard->SetNumSprites(13);
	m_SurfBoard->Init();
	m_SurfBoard->SetShift(0, (GLfloat) 2 / m_SurfBoard->GetNumEffects());

	//Water
	m_Shaders = new Shaders();
	m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");

	m_texture = new Texture();
	m_texture->Init("..\\Data\\Textures\\water256.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	m_model = new Models();
	m_model->Init("..\\Data\\Model\\Sprite2D_1.nfg", NFG);
	for (int i = 0; i < 16; ++i) {
		m_ArrWater.push_back(new Sprite2D(m_model, m_Shaders, m_texture));
	}
	int i = 0, j = 0;
	for (auto& obj : m_ArrWater) {
		obj->SetSize((GLfloat)screenWidth / 2, (GLfloat)screenHeight / 2);
		obj->Set2DPosition(i*screenWidth/2 - (GLfloat)screenWidth / 4,j*screenHeight/2 -(GLfloat)screenHeight / 4);
		obj->Init();
		i++;
		if (i == 4) {
			i = 0;
			j++;
		}
		//printf("%f, %f\n", obj->Get2DPosition().x, obj->Get2DPosition().y);
	}

	//object
	m_Shaders = new Shaders();
	m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");

	m_texture = new Texture();
	m_texture->Init("..\\Data\\Textures\\objects64.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	m_model = new Models();
	m_model->Init("..\\Data\\Model\\Object64.nfg", NFG);
	m_ArrObject.push_back(std::make_shared<Sprite2D>(m_model, m_Shaders, m_texture));
	m_ArrObject.at(0)->SetSize(64, 64);
	m_ArrObject.at(0)->Set2DPosition(320, 360);
	m_ArrObject.at(0)->Init();


	////button
	//m_Shaders = new Shaders();
	//m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");

	//m_texture = new Texture();
	//m_texture->Init("..\\Data\\Textures\\rpg_sprite_walk_1.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	//m_model = new Models();
	//m_model->Init("..\\Data\\Model\\Sprite2D.nfg", NFG);

	//m_Sprite2D = new Sprite2D(m_model,m_Shaders,m_texture, m_Camera);
	//m_Sprite2D->Set2DPosition(300, 300);
	//m_Sprite2D->Init();


	////camera
	////Vector3 CameraPos(10, 40, 40);
	////Vector3 TargetPos(0, 0, 0);
	////float fFovY = 0.7f;
	////m_Camera = new Camera();
	////m_Camera->Init(CameraPos, TargetPos, fFovY, (GLfloat)screenWidth / screenHeight, 1.0f, 5000.0f, 1.0f);


	////plan
	//m_model = new Models();
	//m_model->Init("..\\Data\\Model\\Plan.nfg", NFG);
	//m_Shaders = new Shaders();
	//m_Shaders->Init("..\\Data\\Shaders\\ColorShader.vs", "..\\Data\\Shaders\\TextureShader.fs");
	//m_texture = new Texture();
	//m_texture->Init("..\\Data\\Textures\\Dirt.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	//m_Plan = new Sprite3D(m_model, m_Shaders, m_Camera, m_texture);
	//m_Plan->Set3DScale(Vector3(20, 20, 20));
	//m_Plan->Init();

	////box
	//m_model = new Models();
	//m_model->Init("..\\Data\\Model\\box.nfg", NFG);
	//m_Shaders = new Shaders();
	//m_Shaders->Init("..\\Data\\Shaders\\ColorShader.vs", "..\\Data\\Shaders\\ColorShader.fs");

	//m_Sprite3D = new Sprite3D(m_model, m_Shaders, m_Camera, Vector4(0.0, 0.0, 1.0, 0.5));
	//m_Sprite3D->Set3DScale(Vector3(1, 1, 1));
	//m_Sprite3D->Init();


	////cirle
	//m_model = new Models();
	//m_model->Init("..\\Data\\Model\\Bila.nfg", NFG);
	//m_Shaders = new Shaders();
	//m_Shaders->Init("..\\Data\\Shaders\\TextureShader.vs", "..\\Data\\Shaders\\TextureShader.fs");
	//m_texture = new Texture();
	//m_texture->Init("..\\Data\\Textures\\Rock.tga", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR);

	//m_Sprite3D1 = new Sprite3D(m_model, m_Shaders, m_Camera, m_texture);
	//m_Sprite3D1->Init();
	//m_Sprite3D1->Set3DScale(Vector3(0.05, 0.05, 0.05));
	///*m_Sprite3D1->SetShaders(m_Shaders);
	//m_Sprite3D1->SetModels(m_model);
	//m_Sprite3D1->SetColor(Vector4(0.0, 1.0, 1.0, 0.5));
	//m_Sprite3D1->SetCamera(m_Camera);*/
}

void Application::Update(GLfloat deltaTime)
{
	//update
	this->DoCollision();
	//m_Player->Update(deltaTime);
	//m_SurfBoard->Update(deltaTime)
	m_SurfBoard->Animation(deltaTime);
	for (auto& obj : m_ArrWater) {
		obj->Update(deltaTime);
	}
	m_ArrObject.at(0)->Update(deltaTime);
	/*m_Plan->Update(deltaTime);
	m_Sprite3D->Update(deltaTime);
	m_Sprite2D->Update(deltaTime);
	m_Sprite3D1->Update(deltaTime);*/
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Draw

	//3D
	//m_Plan->Draw();
	//m_Sprite3D->Draw();
	//m_Sprite3D1->Draw();
	
	//2D
	//m_Sprite2D->Draw();
	for (auto& obj : m_ArrWater) {
		obj->Draw();
	}
	m_SurfBoard->Draw();
	m_Player->Draw();
	m_ArrObject.at(0)->Draw();
}

void Application::HandleKeyEvent(unsigned char key, bool bIsPresseded)
{
	//printf("%c", key);
	switch (key) {
	case 'S':
		if (bIsPresseded) {
			m_Player->SetShift((GLfloat)3 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)3 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(0, 2));
			}
			m_ArrObject.at(0)->SetRange(Vector2(0, 2));
		}
		break;
	case 'W':
		m_Player->SetShift(0, m_Player->GetXYShiftPosition().y);
		m_SurfBoard->SetShift(0, m_SurfBoard->GetXYShiftPosition().y);
		for (auto& obj : m_ArrWater) {
			obj->SetRange(Vector2(0, 0));
		}
		break;
	case 'A':
		if (m_Player->GetXYShiftPosition().x == (GLfloat)2 / m_Player->GetNumSprites() && bIsPresseded) {
			m_Player->SetShift((GLfloat)1 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)1 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(-4, 2));
			}
		}
		else if(m_Player->GetXYShiftPosition().x == (GLfloat)1 / m_Player->GetNumSprites() && bIsPresseded){
			m_Player->SetShift((GLfloat)1 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)1 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(-4, 2));
			}
		}
		else if(bIsPresseded){
			m_Player->SetShift((GLfloat)2 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)2 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(-2, 2));
			}
		}
		break;
	case 'D':
		if (m_Player->GetXYShiftPosition().x == (GLfloat)4 / m_Player->GetNumSprites() && bIsPresseded) {
			m_Player->SetShift((GLfloat)5 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)5 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(4, 2));
			}
		}
		else if (m_Player->GetXYShiftPosition().x == (GLfloat)5 / m_Player->GetNumSprites() && bIsPresseded) {
			m_Player->SetShift((GLfloat)5 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)5 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(4, 2));
			}
		}
		else if (bIsPresseded) {
			m_Player->SetShift((GLfloat)4 / m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
			m_SurfBoard->SetShift((GLfloat)4 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
			for (auto& obj : m_ArrWater) {
				obj->SetRange(Vector2(2, 2));
			}
		}
		break;
	}


}

void Application::HandleTouchEvent(GLint x, GLint y, bool bIsPresseded)
{
	//if (x >= m_Sprite2D->Get2DPosition()[0] && x <= m_Sprite2D->Get2DPosition()[0] + 100 && y >= m_Sprite2D->Get2DPosition()[1] && y <= m_Sprite2D->Get2DPosition()[1] + 100) {
	//	//m_Sprite2D->Set2DPosition(x, y); 
	//	printf("%f, %f\n", m_Sprite2D->Get2DPosition()[0], m_Sprite2D->Get2DPosition()[1]);
	//}
}

void Application::DoCollision()
{
	if (m_Player->CheckCollision(m_ArrObject.at(0))) {
		m_ArrObject.at(0)->SetRange(Vector2(0, 0));
		for (auto& obj : m_ArrWater) {
			obj->SetRange(Vector2(0, 0));
		}
		m_Player->SetShift((GLfloat) 6/m_Player->GetNumSprites(), m_Player->GetXYShiftPosition().y);
		m_SurfBoard->SetShift((GLfloat)6 / m_SurfBoard->GetNumSprites(), m_SurfBoard->GetXYShiftPosition().y);
	}
}

void Application::Exit()
{
	exit(0);
}
