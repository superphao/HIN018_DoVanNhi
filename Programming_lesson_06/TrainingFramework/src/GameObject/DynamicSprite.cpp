#include "DynamicSprite.h"
extern GLint screenWidth;
extern GLint screenHeight;

DynamicSprite::DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_Effects = nullptr;
	m_Speed = 5;
}

DynamicSprite::~DynamicSprite()
{
}

void DynamicSprite::Update(GLfloat deltaTime, StateSprite stt)
{
	switch (stt)
	{
	case MOVE_FORWARD:
		Set2DPosition(m_Vec2DPos.x, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_LEFT:
		Set2DPosition(m_Vec2DPos.x + 2 * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_RIGHT:
		Set2DPosition(m_Vec2DPos.x - 2 * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_RIGHT_PLUS:
		Set2DPosition(m_Vec2DPos.x - 4 * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case MOVE_LEFT_PLUS:
		Set2DPosition(m_Vec2DPos.x + 4 * 60 * deltaTime, m_Vec2DPos.y - m_Speed * 60 * deltaTime);
		break;
	case FREEZE:
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
