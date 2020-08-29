#pragma once
#include "Sprite2D.h"
class Player :
    public Sprite2D
{
public:
    Player(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    ~Player();
    void MenuPlayerAnimation(GLfloat deltaTime);
private:
    GLint tmp_Player;
    GLfloat time;
};

