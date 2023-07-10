#pragma once

#include <unordered_map>
#include <string>
#include "raylib.h"

class Cache
{
public:
    Texture2D &getTexture(const std::string &filePath);
    Shader &getShader(const std::string &filePath);

private:
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Shader> shaders;

};
