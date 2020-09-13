#pragma once
#include "Sprite2D.h"

enum StatePlayer
{
    PLAYER_DIE,
    PLAYER_JUMP,
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
    void SetEnergy(GLint energy);
    GLint GetEnergy();
    void SetShadow(std::shared_ptr<Sprite2D> shadow);
    std::shared_ptr<Sprite2D> GetShadow();
    void SetTimeJump(GLfloat time);
    GLfloat GetTimeJump();

private:
    GLint m_NextFrame;
    GLfloat m_time;
    GLint m_Heart;
    GLint m_Energy;
    std::shared_ptr<Sprite2D> m_SurfBoard;
    GLfloat m_TimeDie;
    GLfloat m_TimeProtected;
    GLfloat m_TimeFlicker;
    GLboolean m_isProtected;
    GLfloat m_TimeJump;
    std::shared_ptr<Sprite2D> m_Shadow;
    GLfloat m_fix_alpha;
};

