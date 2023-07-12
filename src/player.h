#pragma once

#include <vector>
#include <cmath>
#include "raylib.h"

class Game;
class ProjectileManager;
class Cache;
class Enemy;
class Tile;

class Player
{
public:
    void init(Game *game, Cache *cache, ProjectileManager *projectileManager, float scale);
    void update(float delta, std::vector<Tile> map, std::vector<Enemy> enemies);
    void draw();

    Vector2 getCenter();
    Vector2 getPos();
    void setRotation(float rotation);
    void setDirection(float direction);
    void setOnTransition(bool onTransition);
    bool isDead();

private:
    Game *game;
    bool onTransition;
    bool dead;
    float scale;
    float rotation;
    float direction;
    float shootCooldown;
    float shootTimer;
    Rectangle rect;
    Texture2D sprite;
    Vector2 facing;
    Vector2 oldPos;

    short UP;
    short DOWN;
    short RIGHT;
    short LEFT;

    ProjectileManager *projectileManager;

    float linearEasing(double start, double end, double duration, double time);
    
};
