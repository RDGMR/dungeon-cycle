#include "projectile.h"
#include "cache.h"

#include <iostream>

Projectile::Projectile(Cache *cache, Vector2 pos, Vector2 direction, float scale)
{
    this->scale = scale;
    this->speed = 600.0f;
    this->rect = { pos.x, pos.y, scale, scale };
    this->direction = direction;
    this->color = GetRandomValue(0, 1) == 0 ? (Color){ 255, 255, 255, 255} : (Color){ 192, 203, 220, 255 };

    rect.x += (direction.x > 0) * scale;
    rect.y += (direction.y == 0) * GetRandomValue(0, 1) * scale;
    rect.y += (direction.y > 0) * scale;
    rect.x += (direction.x == 0) * GetRandomValue(0, 1) * scale;
}

void Projectile::update(float delta)
{
    rect.x += direction.x * speed * delta;
    rect.y += direction.y * speed * delta;
}

void Projectile::draw()
{
    DrawRectangleRec(rect, color);
}

Rectangle Projectile::getRect()
{
    return rect;
}
