#include "projectile.h"
#include "cache.h"

#include <iostream>

Projectile::Projectile(Cache *cache, Vector2 pos, Vector2 direction, float scale)
{
    this->variation = GetRandomValue(1, 4);
    this->speed = 600.0f;
    this->rect = { pos.x, pos.y, 2.0f*scale, 2.0f*scale };
    this->sprite = cache->getTexture("resources/sprites/projectile" + std::to_string(variation) + ".png");
    this->direction = direction;
    if (direction.x == 1.0f && direction.y == 0.0f)
        this->rotation = 0.0f;
    else if (direction.x == 0.0f && direction.y == 1.0f)
        this->rotation = 90.0f;
    else if (direction.x == -1.0f && direction.y == 0.0f)
        this->rotation = 180.0f;
    else if (direction.x == 0.0f && direction.y == -1.0f)
        this->rotation = 270.0f;
}

void Projectile::update(float delta)
{
    rect.x += direction.x * speed * delta;
    rect.y += direction.y * speed * delta;
}

void Projectile::draw()
{
    DrawTexturePro(sprite, { 0.0f, 0.0f, 2.0f, 2.0f }, { rect.x+rect.width/2, rect.y+rect.height/2, rect.width, rect.height }, { rect.width/2, rect.height/2 }, rotation, WHITE);
}

Rectangle Projectile::getRect()
{
    return rect;
}
