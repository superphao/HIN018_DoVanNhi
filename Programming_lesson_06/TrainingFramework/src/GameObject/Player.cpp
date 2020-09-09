#include "Player.h"

Player::Player(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_time = 0;
	m_NextFrame = 1;
	m_Heart = 3;
	m_SurfBoard = nullptr;
	m_TimeDie = 2;
	m_TimeProtected = 0;
	m_isProtected = false;
}

Player::~Player()
{

}

void Player::MenuPlayerAnimation(GLfloat deltaTime)
{
	if (m_time > 1) {
		if (m_CurrentFrame.x == 5)
		{
			m_NextFrame = -1;
		}
		else if (m_CurrentFrame.x == 1)
		{
			m_NextFrame = 1;
		}
		SetFrame(Vector2(m_CurrentFrame.x + m_NextFrame, m_CurrentFrame.y));
		if (m_SurfBoard != nullptr)
			m_SurfBoard->SetFrame(Vector2(m_SurfBoard->GetFrame().x + m_NextFrame, m_SurfBoard->GetFrame().y));
		m_time -= 1;
	}
	m_time = m_time + 4 * deltaTime;
}

void Player::SetSurfBoard(std::shared_ptr<Sprite2D> surfBoard)
{
	m_SurfBoard = surfBoard;
}

std::shared_ptr<Sprite2D> Player::GetSurfBoard()
{
	return m_SurfBoard;
}

void Player::Update(GLfloat deltaTime, StatePlayer stt)
{
	switch (stt)
	{
	case PLAYER_DIE:
		m_TimeProtected = 2;
		if (m_TimeDie == 2) m_Heart--;
		if (m_TimeDie > 1)
		{
			m_CurrentFrame = Vector2(6, m_CurrentFrame.y);
			m_SurfBoard->SetFrame(Vector2(6, m_SurfBoard->GetFrame().y));
			m_TimeDie -= deltaTime;
		}
		else
		{
			if (m_Heart > 0)
			{
				if ((GLint)m_TimeDie % 2 == 0)
				{
					m_CurrentFrame = Vector2(-1, m_CurrentFrame.y);
					m_SurfBoard->SetFrame(Vector2(-1, m_SurfBoard->GetFrame().y));
				}
				else
				{
					m_CurrentFrame = Vector2(0, m_CurrentFrame.y);
					m_SurfBoard->SetFrame(Vector2(0, m_SurfBoard->GetFrame().y));
				}
				m_TimeDie -= deltaTime * 4;
				m_isProtected = true;
			}
		}

		break;
	case PLAYER_PROTECTED:
		break;
	case PLAYER_NORMAL:
		if (m_TimeProtected > 0)
		{
			m_CurrentFrame = Vector2(m_CurrentFrame.x * -1, m_CurrentFrame.y);
			m_SurfBoard->SetFrame(Vector2(m_SurfBoard->GetFrame().x * -1, m_SurfBoard->GetFrame().y));
			m_TimeProtected -= deltaTime;
		}
		else
		{
			if (m_CurrentFrame.x < 0)
			{
				m_CurrentFrame = Vector2(m_CurrentFrame.x * -1, m_CurrentFrame.y);
				m_SurfBoard->SetFrame(Vector2(m_SurfBoard->GetFrame().x * -1, m_SurfBoard->GetFrame().y));
			}
			m_isProtected = false;
		}
		m_TimeDie = 2;
		break;
	default:
		break;
	}
}

GLboolean Player::IsProtected()
{
	return m_isProtected;
}

void Player::SetHeart(GLint heart)
{
	m_Heart = heart;
}

GLint Player::GetHeart()
{
	return m_Heart;
}
