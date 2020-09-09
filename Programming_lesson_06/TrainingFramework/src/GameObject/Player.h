#pragma once
#include "Sprite2D.h"

enum StatePlayer
{
    PLAYER_DIE,
    PLAYER_PROTECTED,
    PLAYER_NORMAL,
};

class Player :
    public Sprite2D
{
public:
    Player(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    ~Player();
    void MenuPlayerAnimation(GLfloat deltaTime);
    void SetSurfBoard(std::shared_ptr<Sprite2D> surfBoard);
    std::shared_ptr<Sprite2D> GetSurfBoard();
    void Update(GLfloat deltaTime, StatePlayer stt);
    GLboolean IsProtected();
    void SetHeart(GLint heart);
    GLint GetHeart();

private:
    GLint m_NextFrame;
    GLfloat m_time;
    GLint m_Heart;
    std::shared_ptr<Sprite2D> m_SurfBoard;
    GLfloat m_TimeDie;
    GLfloat m_TimeProtected;
    GLboolean m_isProtected;
};

