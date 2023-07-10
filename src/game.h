#pragma once

#include "raylib.h"

#include "cache.h"
#include "map.h"
#include "player.h"
#include "projectileManager.h"

class Game
{
public:
    Game();

    void run();
    void update();
    void draw();

    void startTransition(float value);
    void endTransition();
    float easeInOutSine(float start, float end, float duration, float time);
    float easeInCubic(float start, float end, float duration, float time);
    bool onTransition;
    float transitionTimer;
    float transitionDuration;
    float transitionValue;

    bool paused;
    int state;
    Texture2D splashSprite;
    Texture2D deadSprite;
    float timer;

    RenderTexture2D screen;
    int screenWidth;
    int screenHeight;
    float scale;
    int windowWidth;
    int windowHeight;
    float rotation;
    float targetRotation;
    float oldRotation;
    Vector2 origin;

    float delta;

    Cache cache;
    Map map;
    Player player;
    ProjectileManager projectileManager;

};
