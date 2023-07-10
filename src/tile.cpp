#include "tile.h"
#include "cache.h"

#include <iostream>

Tile::Tile(Cache *cache, float rotation, float scale, int id, float x, float y)
{
    this->disabled = false;
    this->solid = false;
    this->visible = true;
    this->trigger = false;
    this->exitTrigger = false;
    this->enterTrigger = false;
    this->direction = 0.0f;
    loadSprite(id);
    this->rect = { x*scale, y*scale, (2.0f+2.0f*trigger)*scale, (2.0f+2.0f*trigger)*scale };
    if (rotation == 0.0f)
    {
        if (enterTrigger){
            rect.x += 2.0f * scale;}
        if (exitTrigger){
            rect.y += 2.0f * scale;}
    }
    if (rotation == 90.0f)
    {
        rect.y += 16.0f * scale;
        if (trigger){
            rect.y -= 2.0f * scale;}
        if (enterTrigger){
            rect.y -= 2.0f * scale;}
        if (exitTrigger){
            rect.x += 2.0f * scale;}
    }
    if (rotation == 180.0f)
    {
        rect.x += 16.0f * scale;
        rect.y += 16.0f * scale;
        if (trigger)
        {
            rect.x -= 2.0f * scale;
            rect.y -= 2.0f * scale;
        }
        if (enterTrigger){
            rect.x -= 2.0f * scale;}
        if (exitTrigger){
            rect.y -= 2.0f * scale;}
    }
    if (rotation == 270.0f)
    {
        rect.x += 16.0f * scale;
        if (trigger){
            rect.x -= 2.0f * scale;}
        if (enterTrigger){
            rect.y += 2.0f * scale;}
        if (exitTrigger){
            rect.x -= 2.0f * scale;}
    }
    this->sprite = cache->getTexture("resources/sprites/tileset.png");
}

void Tile::draw()
{
    if (visible && !disabled)
        DrawTexturePro(sprite, { 0.0f+spriteOffset, 0.0f, 2.0f, 2.0f }, rect, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void Tile::loadSprite(int id)
{
    this->spriteOffset = (id-1) * 2.0f;

    std::ifstream f("resources/maps/tileset.tsj");
    nlohmann::json data = nlohmann::json::parse(f);

    for (auto tile : data["tiles"])
        if (tile["id"] == id-1)
        {
            for (auto property : tile["properties"])
            {
                if      (property["name"] == "solid"){
                    this->solid = property["value"].get<bool>();}
                else if (property["name"] == "visible"){
                    this->visible = property["value"].get<bool>();}
                else if (property["name"] == "trigger"){
                    this->trigger = property["value"].get<bool>();}
                else if (property["name"] == "direction"){
                    this->direction = property["value"].get<float>();}
                else if (property["name"] == "exitTrigger"){
                    this->exitTrigger = property["value"].get<bool>();}
                else if (property["name"] == "enterTrigger"){
                    this->enterTrigger = property["value"].get<bool>();}
            }
        }
}

float Tile::getDirection()
{
    return direction;
}

bool Tile::isSolid()
{
    return solid;
}

bool Tile::isTrigger()
{
    return trigger;
}

Rectangle Tile::getRect()
{
    return this->rect;
}

void Tile::setDisabled(bool disabled)
{
    this->disabled = disabled;
}

// Vector2 Tile::getCenter()
// {
//     return { rect.x + rect.width/2, rect.y + rect.height/2 };
// }
