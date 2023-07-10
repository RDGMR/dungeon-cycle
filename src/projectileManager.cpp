#include "projectileManager.h"
#include "cache.h"
#include "enemy.h"
#include "tile.h"

#include <iostream>

void ProjectileManager::init(Cache *cache, float scale)
{
    this->scale = scale;
    this->cache = cache;
}

void ProjectileManager::update(float delta, std::vector<Tile> map, std::vector<Enemy> *enemies)
{
    // this could be better?
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        it->update(delta);

        bool erase = false;

        for (auto &tile : map)
            if (tile.isSolid() && CheckCollisionRecs(it->getRect(), tile.getRect()))
            {
                erase = true;
                break;
            }
        for (auto &enemy : *enemies)
            if (CheckCollisionRecs(it->getRect(), enemy.getRect()))
            {
                erase = true;
                enemy.damage();
                break;
            }

        if (erase)
            it = projectiles.erase(it);
        else
            it++;
    }
}

void ProjectileManager::draw()
{
    for (auto &projectile : projectiles)
        projectile.draw();
}

void ProjectileManager::shoot(Vector2 pos, Vector2 direction)
{
    projectiles.push_back(Projectile(cache, pos, direction, scale));
}
