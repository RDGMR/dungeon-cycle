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
    int variation;
    float speed;
    float rotation;
    Rectangle rect;
    Texture2D sprite;
    Vector2 direction;

};
