#pragma once
#include "Sprite2D.h"

enum DynamicSpriteType
{
    OBJECT64,
    OBJECT32,
    SANDBAR256,
    AMBIENT64,
    SLOWDOWN64,
    SLOWDOWN192,
    WHIRLPOOL,
    HEART,
    ENERGY,
};

enum StateMoveSprite
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_LEFT_PLUS,
    MOVE_RIGHT_PLUS,
    FREEZE,
};

enum StateSprite
{
    SLOWDOWN,
    NORMAL,
    STOP,
    SPEEDUP,
};

class DynamicSprite :
    public Sprite2D
{
public:
    DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture, GLfloat speed);
    ~DynamicSprite();
    void Update(GLfloat deltaTime, StateMoveSprite stt_move, StateSprite stt_sprite);
    void SetEffects(std::shared_ptr<DynamicSprite> effect);
    std::shared_ptr<DynamicSprite> GetEffects();
    void SetSpeed(GLfloat speed);
    GLfloat GetSpeed();
    void SetMaxSpeed(GLfloat maxSpeed);

protected:
    std::shared_ptr<DynamicSprite> m_Effects;
    GLfloat m_Speed;
    GLfloat m_MaxSpeed;
};

