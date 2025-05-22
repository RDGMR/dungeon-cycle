#include "player.h"
#include "game.h"
#include "projectileManager.h"
#include "cache.h"
#include "enemy.h"
#include "tile.h"

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
    this->sprite = cache->getTexture(ASSETS_PATH "sprites/player.png");
    this->facing = { 1.0f, 0.0f };
    this->oldPos = { rect.x, rect.y };

    this->projectileManager = projectileManager;
}

void Player::update(float delta, std::vector<Tile> map, std::vector<Enemy> enemies)
{
    Vector2 force = { 0.0f, 0.0f };
    
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
            Vector2 input = {
                float((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))),
                float((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) - (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)))
            };

            // Apply movement based on screen rotation
            float angle = -rotation*DEG2RAD;
            float cosA = cosf(angle);
            float sinA = sinf(angle);

            force = {
                input.x * cosA - input.y * sinA,
                input.x * sinA + input.y * cosA
            };
        }
    
        rect.x += force.x * 300.0f * delta;
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
                    if (force.x > 0.0f)
                        rect.x = tileRect.x - rect.width;
                    if (force.x < 0.0f)
                        rect.x = tileRect.x + tileRect.width;
                    break;
                }
            }
        }

        rect.y += force.y * 300.0f * delta;
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
                    if (force.y > 0)
                        rect.y = tileRect.y - rect.height;
                    if (force.y < 0)
                        rect.y = tileRect.y + tileRect.height;
                    break;
                }
            }
        }

        if (std::round(std::abs(force.x + force.y)) == 1)
            facing = force;

        if (!dead && IsKeyDown(KEY_Z) && shootTimer >= shootCooldown)
        {
            shootTimer = 0.0f;
            projectileManager->shoot({ rect.x, rect.y }, facing);
        }

        for (auto &enemy : enemies)
        {
            if (!enemy.isDead() && CheckCollisionRecs(rect, enemy.getRect()))
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
