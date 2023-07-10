#include "cache.h"

Texture2D &Cache::getTexture(const std::string &filePath)
{
    auto it = textures.find(filePath);
    if (it != textures.end())
    {
        return it->second;
    }

    textures[filePath] = LoadTexture(filePath.c_str());

    Texture2D &texture = textures[filePath];
    return texture;
}

Shader &Cache::getShader(const std::string &filePath)
{
    auto it = shaders.find(filePath);
    if (it != shaders.end())
    {
        return it->second;
    }

    shaders[filePath] = LoadShader(0, filePath.c_str());

    Shader &shader = shaders[filePath];
    return shader;
}
