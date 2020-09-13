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
	m_CurrentFrame = Vector2(0, 0);
	m_NumFrame = 1;
	m_NumSprite = 1;
	m_alpha = 0;
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
	m_CurrentFrame = Vector2(0, 0);
	m_NumFrame = 1;
	m_NumSprite = 1;
	m_alpha = 0;
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
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_CurrentFrame");
	if (iTempShaderVaribleGLID != -1)
		glUniform2f(iTempShaderVaribleGLID, m_CurrentFrame.x, m_CurrentFrame.y);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_NumberFrame");	
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, (GLfloat) m_NumFrame);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_NumberSprite");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, (GLfloat) m_NumSprite);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, 1.0);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_fix_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, m_alpha);

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

Vector2 Sprite2D::GetSize()
{
	return Vector2(m_iWidth, m_iHeight);
}

void Sprite2D::SetFrame(Vector2 nextFrame)
{
	m_CurrentFrame = nextFrame;
}

void Sprite2D::SetNumFrame(GLint numFrame)
{
	m_NumFrame = numFrame;
}

void Sprite2D::SetNumSprite(GLint numSprites)
{
	m_NumSprite = numSprites;
}

GLint Sprite2D::GetNumSprite()
{
	return m_NumSprite;
}

GLint Sprite2D::GetNumFrame()
{
	return m_NumFrame;
}

Vector2 Sprite2D::GetFrame()
{
	return m_CurrentFrame;
}

void Sprite2D::Animation(GLfloat deltatime)
{
	if (m_NumFrame > 1)
	{
		if (time > 1) {
			if (m_CurrentFrame.y > 0)
			{
				m_CurrentFrame.y = m_CurrentFrame.y - 1;
				time -= 1;
			}
			else
			{
				m_CurrentFrame.y = m_NumFrame - 1;
			}
		}
		time = time + 3 * deltatime;
	}
}

void Sprite2D::SetColorAlpha(GLfloat alpha)
{
	m_alpha = alpha;
}


bool Sprite2D::CheckCollision(std::shared_ptr<Sprite2D> object)
{
	Vector2 object_coverage(object->m_iWidth / 2, object->m_iHeight / 2);
	Vector2 this_coverage(object->m_iWidth / 2, object->m_iHeight / 2);

	if (this->GetName() == "player")
	{
		this_coverage.x = m_iWidth / 8;
		this_coverage.y = m_iHeight / 4;
	}
	else if (this->GetName() == "harmful")
	{
		this_coverage.x = m_iWidth / 2 - (object->m_iWidth / 9);
		this_coverage.y = m_iHeight / 9;
	}
	else if (this->GetName() == "whirlpool")
	{
		this_coverage.x = m_iWidth / 4;
		this_coverage.y = m_iHeight / 4;
	}

	if (object->GetName() == "harmful")
	{
		object_coverage.x = object->m_iWidth / 2 - (object->m_iWidth / 9);
		object_coverage.y = object->m_iHeight / 9 ;
	}
	else if (object->GetName() == "whirlpool")
	{
		object_coverage.x = object->m_iWidth / 4;
		object_coverage.y = object->m_iHeight / 4;
	}

	bool collisionX = m_Vec2DPos.x + this_coverage.x >= object->m_Vec2DPos.x - object_coverage.x && object->m_Vec2DPos.x + object_coverage.x >= m_Vec2DPos.x - this_coverage.x;
	bool collisionY = m_Vec2DPos.y + this_coverage.y >= object->m_Vec2DPos.y - object_coverage.y && object->m_Vec2DPos.y + object_coverage.y >= m_Vec2DPos.y - this_coverage.y;
	return collisionX && collisionY;
}


