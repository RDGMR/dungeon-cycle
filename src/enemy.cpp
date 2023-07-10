#include "enemy.h"
#include "cache.h"
#include "tile.h"
#include <iostream>

Enemy::Enemy(Cache *cache, int mapOrigin, float scale, float rotation, float x, float y)
{
    this->disabled = false;
    this->paused = false;
    this->mapOrigin = mapOrigin;
    this->hp = 3;
    this->sprite = cache->getTexture("resources/sprites/enemy.png");
    this->rect = { x*scale, y*scale, 2.0f * scale, 2.0f * scale };

    if (rotation == 90.0f)
        rect.y += 16.0f * scale;
    if (rotation == 180.0f)
    {
        rect.x += 16.0f * scale;
        rect.y += 16.0f * scale;
    }
    if (rotation == 270.0f)
        rect.x += 16.0f * scale;
}

void Enemy::update(float delta, Vector2 playerPos, std::vector<Tile> map)
{
    if (!paused)
    {

    Vector2 move = {
        float(rect.x < playerPos.x) - float(rect.x > playerPos.x),
        float(rect.y < playerPos.y) - float(rect.y > playerPos.y)
    };

    rect.x += move.x * 50.0f * delta;
        for (auto tile : map)
        {
            Rectangle tileRect = tile.getRect();
            if (CheckCollisionRecs(rect, tileRect))
            {
                if (tile.isSolid())
                {
                    if (move.x > 0.0f)
                        rect.x = tileRect.x - rect.width;
                    if (move.x < 0.0f)
                        rect.x = tileRect.x + tileRect.width;
                    break;
                }
            }
        }

        rect.y += move.y * 50.0f * delta;
        for (auto tile : map)
        {
            Rectangle tileRect = tile.getRect();
            if (CheckCollisionRecs(rect, tileRect))
            {
                if (tile.isSolid())
                {
                    if (move.y > 0)
                        rect.y = tileRect.y - rect.height;
                    if (move.y < 0)
                        rect.y = tileRect.y + tileRect.height;
                    break;
                }
            }
        }
    }
}

void Enemy::draw()
{
    if (!disabled)
        DrawTexturePro(sprite, { 0.0f, 0.0f, 2.0f, 2.0f }, rect, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void Enemy::setDisabled(bool disabled)
{
    this->disabled = disabled;
}

void Enemy::setPaused(bool paused)
{
    this->paused = paused;
}

int Enemy::getMapOrigin()
{
    return mapOrigin;
}

Rectangle Enemy::getRect()
{
    return rect;
}

void Enemy::damage()
{
    hp--;
}

int Enemy::getHp()
{
    return hp;
}
