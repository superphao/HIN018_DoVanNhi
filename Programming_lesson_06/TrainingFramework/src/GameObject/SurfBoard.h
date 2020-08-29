#pragma once
#include "Sprite2D.h"
class SurfBoard :
    public Sprite2D
{
public:
    SurfBoard(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    ~SurfBoard();
    void MenuSurfAnimation(GLfloat deltaTime);
private:
    GLint tmp_Player;
    GLfloat time;
};

