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
    int getMapOrigin();
    int getHp();

    Rectangle getRect();

private:
    bool disabled;
    bool paused;
    int hp;
    float hurtDuration;
    float hurtTimer;
    float hurtProgress;
    Rectangle rect;
    Texture2D sprite;

};
