#include "map.h"
#include "cache.h"

#include <iostream>

void Map::init(Cache *cache, float scale)
{
    this->cache = cache;
    this->scale = scale;
    this->rotation = 0.0f;
    this->currentMap = 0;
    this->mapQnt = 10;
    loadMap(currentMap);
}

void Map::update(float delta, Vector2 playerPos)
{
    for (auto &enemies : EnemyAreas)
        for (auto &enemy : enemies)
            enemy.update(delta, playerPos, areas[currentMap]);
}

void Map::draw()
{
    for (auto &map : areas)
        for (auto &tile : map)
            tile.draw();
    for (auto &enemies : EnemyAreas)
        for (auto &enemy : enemies)
            enemy.draw();
}

void Map::setRotation(float rotation)
{
    float rot = std::fmod(rotation, 360.0f);
    if (rot < 0)
        rot += 360;
    this->rotation = rot;
}

std::vector<Tile> Map::getMap()
{
    return areas[currentMap];
}

std::vector<Enemy> *Map::getEnemies()
{
    return &EnemyAreas[currentMap];
}

void Map::loadMap(int currentMap)
{
    std::vector<std::vector<std::vector<int>>> mapArray = loadMapArray(currentMap);
    std::vector<Tile> newMap;
    std::vector<Enemy> newEnemies;

    for (int l = 0; l < (int)mapArray.size(); l++)
    {
        int mapHeight = (int)mapArray[l].size();
        for (int y = 0; y < mapHeight; y++)
        {
            int mapWidth = (int)mapArray[l][y].size();
            for (int x = 0; x < mapWidth; x++)
                if (mapArray[l][y][x] != 0)
                {
                    if (mapArray[l][y][x] == 21)
                    {
                        newEnemies.push_back(Enemy(cache, scale, rotation, 2.0f * x, 2.0f * y));
                    }
                    else
                    {
                        Tile newTile = Tile(cache, rotation, scale, mapArray[l][y][x], 2.0f * x, 2.0f * y);
                        newMap.push_back(newTile);
                    }
                }
        }
    }

    areas.push_back(newMap);
    EnemyAreas.push_back(newEnemies);
}

std::vector<std::vector<std::vector<int>>> Map::loadMapArray(int currentMap)
{
    std::ifstream f(TextFormat("resources/maps/%i.json", currentMap));
    nlohmann::json data = nlohmann::json::parse(f);

    int qntLayers = data["layers"].size();
    int qntY = data["height"];
    int qntX = data["width"];
    std::vector<std::vector<std::vector<int>>> mapArray(qntLayers, std::vector<std::vector<int>>(qntY, std::vector<int>(qntX)));

    for (int l = 0; l < qntLayers; l++)
        for (int y = 0; y < qntY; y++)
            for (int x = 0; x < qntX; x++)
                mapArray[l][y][x] = data["layers"][l]["data"][y * qntX + x];

    if      (rotation == 270.0f)
        mapArray = rotateArrayLayers(mapArray);
    else if (rotation == 180.0f)
    {
        mapArray = rotateArrayLayers(mapArray);
        mapArray = rotateArrayLayers(mapArray);
    }
    else if (rotation == 90.0f)
    {
        mapArray = rotateArrayLayers(mapArray);
        mapArray = rotateArrayLayers(mapArray);
        mapArray = rotateArrayLayers(mapArray);
    }


    return mapArray;
}

std::vector<std::vector<std::vector<int>>> Map::rotateArrayLayers(std::vector<std::vector<std::vector<int>>> &array)
{
    int numLayers = array.size();
    int numRows = array[0].size();
    int numCols = array[0][0].size();

    std::vector<std::vector<std::vector<int>>> rotatedArray(numLayers, std::vector<std::vector<int>>(numCols, std::vector<int>(numRows)));

    for (int l = 0; l < numLayers; ++l)
    {
        for (int i = 0; i < numRows; ++i)
        {
            for (int j = 0; j < numCols; ++j)
            {
                rotatedArray[l][j][numRows - 1 - i] = array[l][i][j];
            }
        }
    }

    return rotatedArray;
}

void Map::disablePrevious(float value)
{
    if (value > 0.0f)
    {
        disable(&areas[currentMap - 1]);
        for (auto &enemy : EnemyAreas[currentMap - 1])
            enemy.setDisabled(true);
        
    }
    else
    {
        disable(&areas[currentMap + 1]);
        for (auto &enemy : EnemyAreas[currentMap + 1])
            enemy.setDisabled(true);
    }
}

void Map::nextMap()
{
    currentMap++;
    if (currentMap == (int)areas.size())
    {
        // Create new map
        loadMap(GetRandomValue(1, mapQnt));
    }
    else
    {
        enable(&areas[currentMap]);
    }
    for (auto &enemy : EnemyAreas[currentMap-1])
    {
        enemy.setDisabled(false);
        enemy.setPaused(true);
    }
    for (auto &enemy : EnemyAreas[currentMap])
    {
        enemy.setDisabled(false);
        enemy.setPaused(true);
    }
}

void Map::previousMap()
{
    currentMap--;
    enable(&areas[currentMap]);
    for (auto &enemy : EnemyAreas[currentMap])
    {
        enemy.setDisabled(false);
        enemy.setPaused(true);
    }
    for (auto &enemy : EnemyAreas[currentMap+1])
    {
        enemy.setDisabled(false);
        enemy.setPaused(true);
    }
}

void Map::disable(std::vector<Tile> *map)
{
    for (auto &tile : *map)
        tile.setDisabled(true);
    for (auto &enemy : EnemyAreas[currentMap])
    {
        enemy.setPaused(false);
    }
}

void Map::enable(std::vector<Tile> *map)
{
    for (auto &tile : *map)
        tile.setDisabled(false);
}
