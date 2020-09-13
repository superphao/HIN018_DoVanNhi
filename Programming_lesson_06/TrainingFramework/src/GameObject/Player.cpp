#include "Player.h"

Player::Player(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_time = 0;
	m_NextFrame = 1;
	m_Heart = 3;
	m_Energy = 0;
	m_SurfBoard = nullptr;
	m_Shadow = nullptr;
	m_TimeDie = 2;
	m_TimeProtected = 0;
	m_isProtected = false;
	m_TimeJump = 1;
	m_fix_alpha = 0.2;
	m_TimeFlicker = 0;
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
				m_CurrentFrame = Vector2(0, m_CurrentFrame.y);
				m_SurfBoard->SetFrame(Vector2(0, m_SurfBoard->GetFrame().y));
				if ((GLint)m_TimeDie % 2 == 0)
				{
					SetColorAlpha(0.5);
					m_SurfBoard->SetColorAlpha(0.5);
				}
				else
				{
					SetColorAlpha(0);
					m_SurfBoard->SetColorAlpha(0);
				}
				m_TimeDie -= deltaTime * 4;
				m_isProtected = true;
			}
		}

		break;
	case PLAYER_JUMP:
		m_isProtected = true;
		if (m_TimeJump > 0.6)
		{
			m_Shadow->SetSize(m_Shadow->GetSize().x - 1, m_Shadow->GetSize().y - 1);
			m_Shadow->Set2DPosition(m_Shadow->Get2DPosition() + Vector2(0, 105*deltaTime));
			m_TimeJump -= deltaTime;
			if (m_fix_alpha < 0.8)
			{
				m_fix_alpha += 6*deltaTime / 4;
			}
		}
		else if (m_TimeJump > 0.4)
		{
			m_Shadow->SetSize(m_Shadow->GetSize().x, m_Shadow->GetSize().y);
			m_Shadow->Set2DPosition(m_Shadow->Get2DPosition());
			m_TimeJump -= deltaTime;
		}
		else if (m_TimeJump > 0)
		{
			m_Shadow->SetSize(m_Shadow->GetSize().x + 1 , m_Shadow->GetSize().y + 1);
			m_Shadow->Set2DPosition(m_Shadow->Get2DPosition() - Vector2(0, 105 * deltaTime));
			m_TimeJump -= deltaTime;
			if (m_fix_alpha > 0.2)
			{
				m_fix_alpha -= 6 * deltaTime / 4;
			}
		}
		else
		{
			m_TimeJump = 1;
		}

		m_Shadow->SetColorAlpha(m_fix_alpha);

		if (m_CurrentFrame.x < 9)
		{
			m_CurrentFrame = Vector2(9, m_CurrentFrame.y);
			m_SurfBoard->SetFrame(Vector2(9, m_SurfBoard->GetFrame().y));
		}
		break;
	case PLAYER_NORMAL:
		if (m_TimeProtected > 0)
		{
			if (m_TimeFlicker > 1)
			{
				if (m_alpha == 0)
				{
					SetColorAlpha(0.5);
					m_SurfBoard->SetColorAlpha(0.5);
				}
				else
				{
					SetColorAlpha(0);
					m_SurfBoard->SetColorAlpha(0);
				}
				m_TimeFlicker -= 1;
			}
			m_TimeFlicker += 3*deltaTime;
			m_TimeProtected -= deltaTime/2;
		}
		else
		{
			if (m_alpha > 0)
			{
				SetColorAlpha(0);
				m_SurfBoard->SetColorAlpha(0);
			}
			m_TimeFlicker = 0;
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

void Player::SetEnergy(GLint energy)
{
	m_Energy = energy;
}

GLint Player::GetEnergy()
{
	return m_Energy;
}

void Player::SetShadow(std::shared_ptr<Sprite2D> shadow)
{
	m_Shadow = shadow;
}

std::shared_ptr<Sprite2D> Player::GetShadow()
{
	return m_Shadow;
}

void Player::SetTimeJump(GLfloat time)
{
	m_TimeJump = time;
}

GLfloat Player::GetTimeJump()
{
	return m_TimeJump;
}
