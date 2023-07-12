#pragma once

#include "raylib.h"

class Cache;

class Projectile
{
public:
    Projectile(Cache *cache, Vector2 pos, Vector2 direction, float scale);
    void update(float delta);
    void draw();

    Rectangle getRect();

private:
    float scale;
    float speed;
    Rectangle rect;
    Vector2 direction;
    Color color;

};
