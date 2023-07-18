#pragma once

#include <vector>
#include "raylib.h"

class Cache;
class Tile;

class Enemy
{
public:
    Enemy(Cache *cache, float scale, float rotation, float x, float y);
    void update(float delta, Vector2 playerPos, std::vector<Tile> map);
    void draw();

    void damage();
    void setDisabled(bool disabled);
    void setPaused(bool paused);

    bool isDead();
    Rectangle getRect();

private:
    bool disabled;
    bool paused;
    bool dead;
    int hp;
    float hurtDuration;
    float hurtTimer;
    float hurtProgress;
    Rectangle rect;
    Texture2D sprite;

};
