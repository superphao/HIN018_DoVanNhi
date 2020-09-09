#pragma once
#include "baseobject.h"

class Sprite2D : public BaseObject
{
private:
	std::string		m_Text;
	void			CaculateWorldMatrix();
	GLint			temp;
	GLfloat			time;
	
protected:
	Vector2			m_Vec2DPos;
	GLint			m_iHeight;
	GLint			m_iWidth;
	Vector2         m_CurrentFrame;
	GLint			m_NumSprite;
	GLint			m_NumFrame;

public:
	Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
	Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, Vector4 color);
	~Sprite2D();

	void		Init() override;
	void		Draw() override;
	void		Update(GLfloat deltatime) override;

	void		SetText(std::string text);
	std::string	GetText();

	void		Set2DPosition(GLfloat x, GLfloat y);
	void		Set2DPosition(Vector2 pos);

	Vector2		Get2DPosition();
	void		SetSize(GLint width, GLint height);

	void		SetFrame(Vector2 nextFrame);
	void		SetNumFrame(GLint numFrame);
	void		SetNumSprite(GLint numSprites);
	GLint		GetNumSprite();
	GLint		GetNumFrame();
	Vector2		GetFrame();
	void		Animation(GLfloat deltatime);

	//xu ly va cham
	bool CheckCollision(std::shared_ptr<Sprite2D> object);
};

