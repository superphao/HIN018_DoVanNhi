#pragma once
#include "baseobject.h"

class Sprite2D : public BaseObject
{
private:
	std::string		m_Text;
	void			CaculateWorldMatrix();
	GLfloat			m_xShift;
	GLfloat			m_yShift;
	GLint			m_numSprites;
	GLint			m_numEffects;
	Vector2			m_Range;
	GLint			temp = 1;
protected:
	Vector2			m_Vec2DPos;
	GLint			m_iHeight;
	GLint			m_iWidth;

public:
	Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
	Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, Vector4 color);
	~Sprite2D();

	void		Init() override;
	void		Draw() final;
	void		Update(GLfloat deltatime) override;

	void		SetText(std::string text);
	std::string	GetText();

	void		Set2DPosition(GLfloat x, GLfloat y);
	void		Set2DPosition(Vector2 pos);

	Vector2		Get2DPosition();
	void		SetSize(GLint width, GLint height);
	void		SetNumEffects(GLint numEffects);
	void		SetNumSprites(GLint numSprites);
	GLint		GetNumSprites();
	GLint		GetNumEffects();
	void		Move(Vector2 range, GLfloat deltaTime);
	void		Animation(GLfloat deltatime);
	void		SetRange(Vector2 range);
	Vector2		GetRange();

	//set toa do lay sprite mac dinh
	//sprite mac dinh lay o vi tri cuoi cung ben trai
	//sprite[i][j]:  xShift = i/(so sprite tren 1 hang); yShift = j/(so sprite tren cot)
	void		SetShift(GLfloat xShift, GLfloat yShift);

	//tra ve i, j trong sprite[i][j]
	Vector2		GetXYShiftPosition();

	//xu ly va cham
	bool CheckCollision(std::shared_ptr<Sprite2D> object);
};

