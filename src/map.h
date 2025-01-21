#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "tile.h"
#include "enemy.h"
#include "nlohmann/json.hpp"

class Cache;

class Map
{
public:
    void init(Cache *cache, float scale);
    void update(float delta, Vector2 playerPos);
    void draw();

    // void setOnTransition(bool onTransition);
    void disablePrevious(float value);
    void nextMap();
    void previousMap();
    void setRotation(float rotation);
    std::vector<Tile> getMap();
    std::vector<Enemy> *getEnemies();

    void disable(std::vector<Tile> *map);
    void enable(std::vector<Tile> *map);

private:
    Cache *cache;
    // bool onTransition;
    float scale;
    float rotation;
    int currentMap;
    int mapQnt;

    void loadMap(int currentMap);
    std::vector<std::vector<std::vector<int>>> loadMapArray(int currentMap);
    std::vector<std::vector<std::vector<int>>> rotateArrayLayers(std::vector<std::vector<std::vector<int>>> &array);
    std::vector<std::vector<Tile>> areas;
    std::vector<std::vector<Enemy>> EnemyAreas;

};
