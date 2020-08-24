#include "Sprite2D.h"
#include "Shaders.h"
#include "Models.h"
#include "Camera.h"
#include "Texture.h"

extern GLint screenWidth;
extern GLint screenHeight;
GLfloat xShift = 0;
GLfloat yShift = 0;
GLint time = 0;
GLint temp = 0;

void Sprite2D::CaculateWorldMatrix()
{
	Matrix m_Sc, m_T;
	m_Sc.SetScale(m_Vec3Scale);
	m_T.SetTranslation(m_Vec3Position);
	m_WorldMat = m_Sc*m_T;
}

Sprite2D::Sprite2D(Models * model, Shaders * shader, Texture * texture)
	: BaseObject()
{
	m_pModel = model;
	m_pShader = shader;
	m_pCamera = nullptr;
	m_pTexture = texture;

	m_Vec3Position = Vector3(0, 0, 0);
	m_iHeight = 30;
	m_iWidth = 40;
	m_Vec3Scale = Vector3((float)m_iWidth/screenWidth, (float)m_iHeight/screenHeight, 1);
}

Sprite2D::Sprite2D(Models * model, Shaders * shader, Vector4 color)
	: BaseObject()
{
	m_pModel = model;
	m_pShader = shader;
	m_pCamera = nullptr;
	m_pTexture = nullptr;
	m_Color = color;

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
		//glUniform2f(glGetUniformLocation(m_pShader->program, "a_uv") , 0.125, 0.125);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, 1.0);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"temp");
	if (iTempShaderVaribleGLID != -1)
		glUniform2f(iTempShaderVaribleGLID, xShift, yShift);

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
	/*printf("%f", xShift);
	if (time % 10 == 0) {
		if (temp == 0)
			if (xShift < 0.875) {
				xShift = xShift + 0.125;
				if (xShift == 0.875) temp = 1;
			}
		if (temp == 1)
			if (xShift > 0) {
				xShift = xShift - 0.125;
				if (xShift == 0) temp = 0;
			}
	}
	if (m_Vec2DPos.x >= 640) m_Vec2DPos.x = 0;
	float xx = m_Vec2DPos.x + 1;
	float yy = m_Vec2DPos.y;
	Set2DPosition(xx, yy);
	time = time + 1;*/
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

	GLfloat xx = (2.0 * m_Vec2DPos.x) / screenWidth - 1.0;
	GLfloat yy = 1.0 - (2.0*m_Vec2DPos.y) / screenHeight;
	m_Vec3Position = Vector3(xx, yy, 1.0);

	CaculateWorldMatrix();
}

void Sprite2D::Set2DPosition(Vector2 pos)
{
	m_Vec2DPos = pos;

	float xx = (2.0 * m_Vec2DPos.x) / screenWidth - 1.0;
	float yy = 1.0 - (2.0*m_Vec2DPos.y) / screenHeight;
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

void Sprite2D::setTexture(Texture* texture) {
	m_pTexture = texture;
}

void Sprite2D::Move() {
	printf("%f", xShift);
	if (time % 5 == 0) {
		if (temp == 0)
			if (xShift < 0.875) {
				xShift = xShift + 0.125;	
				if (xShift == 0.875) temp = 1;
			}
		if (temp == 1)
			if (xShift > 0) {
				xShift = xShift - 0.125;
				if (xShift == 0) temp = 0;
			}
	}
	if (m_Vec2DPos.x >= 640) m_Vec2DPos.x = 0;
	float xx = m_Vec2DPos.x + 2;
	float yy = m_Vec2DPos.y;
	Set2DPosition(xx, yy);
	time = time + 1;
}