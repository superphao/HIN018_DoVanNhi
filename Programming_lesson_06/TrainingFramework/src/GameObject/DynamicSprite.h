#pragma once
#include "Sprite2D.h"

enum DynamicSpriteType
{
    OBJECT64,
    OBJECT32,
    SANDBAR256,
};

enum StateSprite
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_LEFT_PLUS,
    MOVE_RIGHT_PLUS,
    FREEZE,
};

class DynamicSprite :
    public Sprite2D
{
public:
    DynamicSprite(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    ~DynamicSprite();
    void Update(GLfloat deltaTime, StateSprite stt);
    void SetEffects(std::shared_ptr<DynamicSprite> effect);
    std::shared_ptr<DynamicSprite> GetEffects();
    void SetSpeed(GLfloat speed);
    GLfloat GetSpeed();

protected:
    std::shared_ptr<DynamicSprite> m_Effects;
    GLfloat m_Speed;
};

