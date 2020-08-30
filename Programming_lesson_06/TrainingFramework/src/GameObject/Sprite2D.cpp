#include "Sprite2D.h"
#include "Shaders.h"
#include "Models.h"
#include "Camera.h"
#include "Texture.h"

extern GLint screenWidth;
extern GLint screenHeight;


void Sprite2D::CaculateWorldMatrix()
{
	Matrix m_Sc, m_T;
	m_Sc.SetScale(m_Vec3Scale);
	m_T.SetTranslation(m_Vec3Position);
	m_WorldMat = m_Sc * m_T;
}

Sprite2D::Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: BaseObject()
{
	m_pModel = model;
	m_pShader = shader;
	m_pCamera = nullptr;
	m_pTexture = texture;
	m_xShift = 0;
	m_yShift = 0;
	time = 0;
	temp = 1;

	m_Vec3Position = Vector3(0, 0, 0);
	m_iHeight = 50;
	m_iWidth = 100;
	m_Vec3Scale = Vector3((float)m_iWidth / screenWidth, (float)m_iHeight / screenHeight, 1);
}

Sprite2D::Sprite2D(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, Vector4 color)
	: BaseObject()
{
	m_pModel = model;
	m_pShader = shader;
	m_pCamera = nullptr;
	m_pTexture = nullptr;
	m_Color = color;
	m_xShift = 0;
	m_yShift = 0;
	time = 0;
	temp = 1;

	m_Vec3Position = Vector3(0, 0, 0);
	m_iHeight = 50;
	m_iWidth = 100;
	m_Vec3Scale = Vector3((float)m_iWidth / screenWidth, (float)m_iHeight / screenHeight, 1);
}

Sprite2D::~Sprite2D()
{
}

void Sprite2D::Init()
{
	CaculateWorldMatrix();
}

void Sprite2D::Draw()
{
	glUseProgram(m_pShader->program);
	glBindBuffer(GL_ARRAY_BUFFER, m_pModel->GetVertexObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModel->GetIndiceObject());

	GLuint iTempShaderVaribleGLID = -1;
	Matrix matrixWVP;

	matrixWVP = m_WorldMat;//* m_pCamera->GetLookAtCamera();

	if (m_pTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->Get2DTextureAdd());
		if (m_pShader->iTextureLoc[0] != -1)
			glUniform1i(m_pShader->iTextureLoc[0], 0);
	}
	else
	{
		iTempShaderVaribleGLID = -1;
		iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_color");
		if (iTempShaderVaribleGLID != -1)
			glUniform4f(iTempShaderVaribleGLID, m_Color.x, m_Color.y, m_Color.z, m_Color.w);
	}


	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*)"a_posL");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_POSITION);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*) "a_uv");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_UV);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*) "temp");
	if (iTempShaderVaribleGLID != -1)
		glUniform2f(iTempShaderVaribleGLID, m_xShift, -m_yShift);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, 1.0);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_matMVP");
	if (iTempShaderVaribleGLID != -1)
		glUniformMatrix4fv(iTempShaderVaribleGLID, 1, GL_FALSE, matrixWVP.m[0]);



	glDrawElements(GL_TRIANGLES, m_pModel->GetNumIndiceObject(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Sprite2D::Update(GLfloat deltatime)
{
	Move(m_Range, deltatime);
}



void Sprite2D::SetText(std::string text)
{
	m_Text = text;
}

std::string Sprite2D::GetText()
{
	return m_Text;
}

void Sprite2D::Set2DPosition(GLfloat width, GLfloat height)
{
	m_Vec2DPos.x = width;
	m_Vec2DPos.y = height;

	float xx = (2.0 * m_Vec2DPos.x) / screenWidth - 1.0;
	float yy = 1.0 - (2.0 * m_Vec2DPos.y) / screenHeight;
	m_Vec3Position = Vector3(xx, yy, 1.0);

	CaculateWorldMatrix();
}

void Sprite2D::Set2DPosition(Vector2 pos)
{
	m_Vec2DPos = pos;

	float xx = (2.0 * m_Vec2DPos.x) / screenWidth - 1.0;
	float yy = 1.0 - (2.0 * m_Vec2DPos.y) / screenHeight;
	m_Vec3Position = Vector3(xx, yy, 1.0);

	CaculateWorldMatrix();
}

Vector2 Sprite2D::Get2DPosition()
{
	return m_Vec2DPos;
}


void Sprite2D::SetSize(GLint width, GLint height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_Vec3Scale = Vector3((float)m_iWidth / screenWidth, (float)m_iHeight / screenHeight, 1);
	CaculateWorldMatrix();
}


void Sprite2D::SetNumEffects(GLint numEffects)
{
	m_numEffects = numEffects;
}

void Sprite2D::SetNumSprites(GLint numSprites)
{
	m_numSprites = numSprites;
}

GLint Sprite2D::GetNumSprites()
{
	return m_numSprites;
}

GLint Sprite2D::GetNumEffects()
{
	return m_numEffects;
}

void Sprite2D::Move(Vector2 range, GLfloat deltaTime) {
	Set2DPosition(m_Vec2DPos.x - range.x * 60 * deltaTime, m_Vec2DPos.y - range.y * 60 * deltaTime);
	if (m_Vec2DPos.x > screenWidth + (GLfloat)screenWidth / 2) m_Vec2DPos.x = -(GLfloat)screenWidth / 2;
	//if (m_Vec2DPos.y > screenHeight) m_Vec2DPos.y = 0;
	if (m_Vec2DPos.x < -(GLfloat)screenWidth / 2) m_Vec2DPos.x = screenWidth + (GLfloat)screenWidth / 2;
	if (m_Vec2DPos.y < -(GLfloat)screenHeight / 2) m_Vec2DPos.y = screenHeight + (GLfloat)screenHeight / 2;
}

void Sprite2D::Animation(GLfloat deltatime)
{
	if (time > 1) {
		if (temp <= m_numEffects - 1) {
			m_yShift = m_yShift - (GLfloat)1 / m_numEffects;
		}
		else if (temp > m_numEffects - 1) {
			m_yShift = m_yShift + (GLfloat)1 / m_numEffects;
		}
		temp = temp % (2 * m_numEffects - 2) + 1;
		time = 0;
	}
	time = time + 3*deltatime;
}

void Sprite2D::SetRange(Vector2 range)
{
	m_Range = range;
}

Vector2 Sprite2D::GetRange()
{
	return m_Range;
}

void Sprite2D::SetShift(GLfloat xShift, GLfloat yShift)
{
	m_xShift = xShift;
	m_yShift = yShift;
}

Vector2 Sprite2D::GetXYShiftPosition()
{
	return Vector2(m_xShift, m_yShift);
}

bool Sprite2D::CheckCollision(std::shared_ptr<Sprite2D> object)
{
	bool collisionX = m_Vec2DPos.x + m_iWidth / 2 >= object->m_Vec2DPos.x - object->m_iWidth/2 && object->m_Vec2DPos.x + object->m_iWidth / 2 >= m_Vec2DPos.x - m_iWidth/2;
	bool collisionY = m_Vec2DPos.y + m_iHeight / 2 - m_iHeight / 4 >= object->m_Vec2DPos.y - m_iHeight/2 + m_iHeight / 4 && object->m_Vec2DPos.y + object->m_iHeight / 2 - m_iHeight / 4 >= m_Vec2DPos.y - m_iHeight/2 + m_iHeight / 4;
	return collisionX && collisionY;
}


