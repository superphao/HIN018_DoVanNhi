#pragma once
#include "Sprite2D.h"

enum EnemyType
{
    OBJECT64,
    OBJECT32,
    SANDBAR256,
};

class Enemy :
    public Sprite2D
{
public:
    Enemy(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture);
    ~Enemy();
    void SetEffects(std::shared_ptr<Sprite2D> effect);
    std::shared_ptr<Sprite2D> GetEffects();
    void SetName(std::string name);
    std::string GetName();

private:
    std::shared_ptr<Sprite2D> m_Effects;
    std::string m_Name;
};

