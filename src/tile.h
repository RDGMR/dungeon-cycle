#pragma once

#include <algorithm>
#include <fstream>
#include "raylib.h"
#include "../libs/json/single_include/nlohmann/json.hpp"

class Cache;

class Tile
{
public:
    Tile(Cache *cache, float rotation, float scale, int id, float x, float y);
    void draw();

    Rectangle getRect();
    float getDirection();
    void setDisabled(bool disabled);
    bool isSolid();
    bool isTrigger();

private:
    int spriteOffset;
    float scale;
    float rotation;
    float direction;
    bool disabled;
    bool solid;
    bool visible;
    bool trigger;
    bool exitTrigger;
    bool enterTrigger;
    Rectangle rect;
    Texture2D sprite;

    void loadSprite(int id);

};
