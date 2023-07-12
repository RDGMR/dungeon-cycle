#include "player.h"
#include "game.h"
#include "projectileManager.h"
#include "cache.h"
#include "enemy.h"
#include "tile.h"

#include <iostream>

void Player::init(Game *game, Cache *cache, ProjectileManager *projectileManager, float scale)
{
    this->game = game;
    this->dead = false;
    this->onTransition = false;
    this->scale = scale;
    this->rotation = 0.0f;
    this->direction = 90.0f;
    this->shootCooldown = 0.5f;
    this->shootTimer = shootCooldown;
    this->rect = { 7.0f*scale, 7.0f*scale, 2.0f*scale, 2.0f*scale };
    this->sprite = cache->getTexture("resources/sprites/player.png");
    this->facing = { 1.0f, 0.0f };
    this->oldPos = { rect.x, rect.y };

    this->UP = KEY_UP;
    this->DOWN = KEY_DOWN;
    this->RIGHT = KEY_RIGHT;
    this->LEFT = KEY_LEFT;

    this->projectileManager = projectileManager;
}

void Player::update(float delta, std::vector<Tile> map, std::vector<Enemy> enemies)
{
    Vector2 move = { 0.0f, 0.0f };
    
    if (onTransition)
    {
        if (rotation == 0.0f)
        {
            if (direction > 0)
                rect.y = linearEasing(oldPos.y, oldPos.y - 3.0f * scale, game->transitionDuration, game->transitionTimer);
            else
                rect.x = linearEasing(oldPos.x, oldPos.x - 3.0f * scale, game->transitionDuration, game->transitionTimer);
        }
        else if (rotation == 90.0f)
        {
            if (direction > 0)
                rect.x = linearEasing(oldPos.x, oldPos.x - 3.0f * scale, game->transitionDuration, game->transitionTimer);
            else
                rect.y = linearEasing(oldPos.y, oldPos.y + 3.0f * scale, game->transitionDuration, game->transitionTimer);
        }
        else if (rotation == 180.0f)
        {
            if (direction > 0)
                rect.y = linearEasing(oldPos.y, oldPos.y + 3.0f * scale, game->transitionDuration, game->transitionTimer);
            else
                rect.x = linearEasing(oldPos.x, oldPos.x + 3.0f * scale, game->transitionDuration, game->transitionTimer);
        }
        else if (rotation == 270.0f)
        {
            if (direction > 0)
                rect.x = linearEasing(oldPos.x, oldPos.x + 3.0f * scale, game->transitionDuration, game->transitionTimer);
            else
                rect.y = linearEasing(oldPos.y, oldPos.y - 3.0f * scale, game->transitionDuration, game->transitionTimer);
        }
    }
    else
    {
        if (!dead)
        {
            move.x += IsKeyDown(RIGHT) - IsKeyDown(LEFT);
            move.y += IsKeyDown(DOWN) - IsKeyDown(UP);
        }
    
        rect.x += move.x * 300.0f * delta;
        for (auto tile : map)
        {
            Rectangle tileRect = tile.getRect();
            if (CheckCollisionRecs(rect, tileRect))
            {
                if (tile.isTrigger() && !onTransition)
                {
                    game->startTransition(90.0f * tile.getDirection());
                }
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

        rect.y += move.y * 300.0f * delta;
        for (auto tile : map)
        {
            Rectangle tileRect = tile.getRect();
            if (CheckCollisionRecs(rect, tileRect))
            {
                if (tile.isTrigger() && !onTransition)
                {
                    game->startTransition(90.0f * tile.getDirection());
                }
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

        if (std::abs(move.x+move.y) == 1)
            facing = move;

        if (!dead && IsKeyDown(KEY_Z) && shootTimer >= shootCooldown)
        {
            shootTimer = 0.0f;
            projectileManager->shoot({ rect.x, rect.y }, facing);
        }

        for (auto &enemy : enemies)
        {
            if (CheckCollisionRecs(rect, enemy.getRect()))
                dead = true;
        }

    }
    
    shootTimer += delta;
}

void Player::draw()
{
    DrawTexturePro(sprite, { 0.0f, 0.0f, 2.0f, 2.0f }, rect, { 0.0f, 0.0f }, 0.0f, WHITE);
}

Vector2 Player::getCenter()
{
    return { rect.x + rect.width/2, rect.y + rect.height/2 };
}

Vector2 Player::getPos()
{
    return { rect.x, rect.y };
}

void Player::setRotation(float rotation)
{
    float rot = std::fmod(rotation, 360.0f);
    if (rot < 0)
        rot += 360;
    this->rotation = rot;

    if      (rot == 0.0f)
    {
        UP = KEY_UP;
        DOWN = KEY_DOWN;
        RIGHT = KEY_RIGHT;
        LEFT = KEY_LEFT;
    }
    else if (rot == 90.0f)
    {
        UP = KEY_RIGHT;
        DOWN = KEY_LEFT;
        RIGHT = KEY_DOWN;
        LEFT = KEY_UP;
    }
    else if (rot == 180.0f)
    {
        UP = KEY_DOWN;
        DOWN = KEY_UP;
        RIGHT = KEY_LEFT;
        LEFT = KEY_RIGHT;
    }
    else if (rot == 270.0f)
    {
        UP = KEY_LEFT;
        DOWN = KEY_RIGHT;
        RIGHT = KEY_UP;
        LEFT = KEY_DOWN;
    }
}

void Player::setDirection(float direction)
{
    this->direction = direction;
}

float Player::linearEasing(double start, double end, double duration, double time)
{
    float x = time / duration;
    return start + (end - start) * x;
}

void Player::setOnTransition(bool onTransition)
{
    this->onTransition = onTransition;
    if (onTransition)
    {
        this->oldPos = { rect.x, rect.y };
    }
}

bool Player::isDead()
{
    return dead;
}
