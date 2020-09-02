#include "SurfBoard.h"

SurfBoard::SurfBoard(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	:Sprite2D(model, shader, texture)
{
	time = 0;
	tmp_Player = 3;
}

SurfBoard::~SurfBoard()
{

}

void SurfBoard::MenuSurfAnimation(GLfloat deltaTime)
{
	if (time > 1) {
		//printf("%f", m_CurrentFrame.x);
		if (tmp_Player <= m_NumSprite - 9) {
			//SetFrame(Vector2((m_CurrentFrame.x, m_CurrentFrame.y - (GLfloat)1 / m_NumSprite);
			SetFrame(Vector2(m_CurrentFrame.x - 1, m_CurrentFrame.y));
		}
		else if (tmp_Player > m_NumSprite - 9) {
			//SetFrame(Vector2((m_CurrentFrame.x, m_CurrentFrame.y + (GLfloat)1 / m_NumSprite);
			SetFrame(Vector2(m_CurrentFrame.x + 1, m_CurrentFrame.y));
		}
		tmp_Player = tmp_Player % (2 * m_NumSprite - 16 - 2) + 1;
		time -= 1;
	}
	time = time + 4*deltaTime;
}
