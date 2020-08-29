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
		//printf("%f", GetXYShiftPosition().x);
		if (tmp_Player <= GetNumSprites() - 9) {
			//SetShift(GetXYShiftPosition().x, GetXYShiftPosition().y - (GLfloat)1 / GetNumSprites());
			SetShift(GetXYShiftPosition().x - (GLfloat)1 / GetNumSprites(), GetXYShiftPosition().y);
		}
		else if (tmp_Player > GetNumSprites() - 9) {
			//SetShift(GetXYShiftPosition().x, GetXYShiftPosition().y + (GLfloat)1 / GetNumSprites());
			SetShift(GetXYShiftPosition().x + (GLfloat)1 / GetNumSprites(), GetXYShiftPosition().y);
		}
		tmp_Player = tmp_Player % (2 * GetNumSprites() - 16 - 2) + 1;
		time = 0;
	}
	time = time + 4*deltaTime;
}
