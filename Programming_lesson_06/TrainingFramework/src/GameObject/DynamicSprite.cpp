#include "DynamicSprite.h"
extern GLint screenWidth;
extern GLint screenHeight;

DynamicSprite::DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_Effects = nullptr;
	m_Speed = 0;
	m_MaxSpeed = 0;
}

DynamicSprite::DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture, GLfloat speed)
	: Sprite2D(model, shader, texture)
{
	m_Effects = nullptr;
	m_Speed = 1;
	m_MaxSpeed = speed;
}


DynamicSprite::~DynamicSprite()
{
}

void DynamicSprite::Update(GLfloat deltaTime, StateMoveSprite stt_move, StateSprite stt_sprite)
{
	switch (stt_sprite)
	{
	case SLOWDOWN:
		m_Speed = m_MaxSpeed - 3;
		break;
	case NORMAL:
		m_Speed = m_MaxSpeed;
		break;
	case SPEEDUP:
		if (m_Speed < m_MaxSpeed)
		{
			m_Speed += deltaTime;
		}
		else
		{
			m_Speed = m_MaxSpeed;
		}
		break;
	case STOP:
		m_Speed = 0;
		break;
	default:
		break;
	}

	switch (stt_move)
	{
	case MOVE_FORWARD:
		Set2DPosition(m_Vec2DPos.x, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_LEFT:
		Set2DPosition(m_Vec2DPos.x + 2 * (m_Speed / 7) * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_RIGHT:
		Set2DPosition(m_Vec2DPos.x - 2 * (m_Speed / 7) * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_RIGHT_PLUS:
		Set2DPosition(m_Vec2DPos.x - 4 * (m_Speed / 7) * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_LEFT_PLUS:
		Set2DPosition(m_Vec2DPos.x + 4 * (m_Speed / 7) * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case FREEZE:
		m_Speed = 0;
		Set2DPosition(m_Vec2DPos.x, m_Vec2DPos.y);
		break;
	default:
		Set2DPosition(m_Vec2DPos.x, m_Vec2DPos.y);
		break;
	}


	if (GetName() == "water") {
		if (m_Vec2DPos.x > (GLfloat)screenWidth + (GLfloat)screenWidth / 2) m_Vec2DPos.x = -(GLfloat)screenWidth / 2;
		//if (m_Vec2DPos.y > screenHeight) m_Vec2DPos.y = 0;
		if (m_Vec2DPos.x < -(GLfloat)screenWidth / 2) m_Vec2DPos.x = screenWidth + (GLfloat)screenWidth / 2;
		if (m_Vec2DPos.y < -(GLfloat)screenHeight / 2) m_Vec2DPos.y = screenHeight + (GLfloat)screenHeight / 2;
	}
}

void DynamicSprite::SetEffects(std::shared_ptr<DynamicSprite> effect)
{
	m_Effects = effect;
}

std::shared_ptr<DynamicSprite> DynamicSprite::GetEffects()
{
	return m_Effects;
}

void DynamicSprite::SetSpeed(GLfloat speed)
{
	m_Speed = speed;
}

GLfloat DynamicSprite::GetSpeed()
{
	return m_Speed;
}

void DynamicSprite::SetMaxSpeed(GLfloat maxSpeed)
{
	m_MaxSpeed = maxSpeed;
}
