#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_Effects = nullptr;
}

Enemy::~Enemy()
{
}

void Enemy::SetEffects(std::shared_ptr<Sprite2D> effect)
{
	m_Effects = effect;
}

std::shared_ptr<Sprite2D> Enemy::GetEffects()
{
	return m_Effects;
}
