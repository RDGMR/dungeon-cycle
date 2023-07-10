#pragma once

#include <vector>
#include "raylib.h"
#include "projectile.h"

class Cache;
class Enemy;
class Tile;

class ProjectileManager
{
public:
    void init(Cache *cache, float scale);
    void update(float delta, std::vector<Tile> map, std::vector<Enemy> *enemies);
    void draw();

    void shoot(Vector2 pos, Vector2 direction);

private:
    float scale;
    Cache *cache;

    std::vector<Projectile> projectiles;

};
