#include "game.h"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game *game;

#if defined(PLATFORM_WEB)
    static void loop()
    {
        game->update();
        game->draw();
    }
#endif

Game::Game()
{
    game = this;
    this->state = 0;
    this->screenWidth = 16;
    this->screenHeight = 16;
    this->scale = 50.0f;
    this->windowWidth = screenWidth * scale;
    this->windowHeight = screenHeight * scale;
    this->rotation = 0.0f;
    this->targetRotation = rotation;
    this->oldRotation = rotation;
    this->origin = { (float)windowWidth, (float)windowHeight };
    this->delta = 0.0f;

    this->onTransition = false;
    this->transitionTimer = 0.0f;
    this->transitionDuration = 2.0f;
    this->transitionValue = 0.0f;

    this->paused = false;
    this->timer = 0.0f;
}

void Game::run()
{
    InitWindow(windowWidth, windowHeight, "Dungeon Cycle");
    this->screen = LoadRenderTexture(windowWidth*2.0f, windowHeight*2.0f);
    this->splashSprite = LoadTexture(ASSETS_PATH "sprites/splash.png");
    this->deadSprite = LoadTexture(ASSETS_PATH "sprites/dead.png");

    map.init(&cache, scale);
    player.init(this, &cache, &projectileManager, scale);
    projectileManager.init(&cache, scale);

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(loop, 0, 1);
    #else
        SetTargetFPS(144);

        while (!WindowShouldClose())
        {
            update();
            draw();
        }
    #endif

    CloseWindow();
}

void Game::update()
{
    if (state == 0)
    {
        // SPLASH
        timer += delta;
        if (timer >= 2.5f)
            state++;
    }
    if (state == 1)
    {
        // PLAY
        if (!paused)
        {
            player.update(delta, map.getMap(), *map.getEnemies());
            map.update(delta, player.getPos());
            projectileManager.update(delta, map.getMap(), map.getEnemies());
        }

        if (onTransition)
        {
            rotation = easeInCubic(oldRotation, targetRotation, transitionDuration, transitionTimer);
            if (transitionTimer > transitionDuration)
                endTransition();
            transitionTimer += delta;
        }
    }
}

void Game::draw()
{
    delta = GetFrameTime();

    if (state == 0)
    {
        // SPLASH
        BeginTextureMode(screen);
            ClearBackground({ 24, 20, 37, 255 });
            DrawTexturePro(splashSprite, { 0.0f, 0.0f, 16.0f, 16.0f }, { 0.0f, 0.0f, 16.0f * scale, 16.0f * scale }, { 0.0f, 0.0f }, 0.0f, WHITE);
        EndTextureMode();

        BeginDrawing();
            ClearBackground({ 24, 20, 37, 255 });
            DrawTexturePro(screen.texture, { 0.0f, 0.0f, (float)screen.texture.width, -(float)screen.texture.height }, { origin.x, origin.y, (float)windowWidth*2.0f, (float)windowHeight*2.0f }, origin, rotation, WHITE);
        EndDrawing();
    }
    if (state == 1)
    {
        // PLAY
        BeginTextureMode(screen);
            ClearBackground({ 24, 20, 37, 255 });
            map.draw();
            projectileManager.draw();
            player.draw();
        EndTextureMode();
        
        BeginDrawing();
            ClearBackground({ 24, 20, 37, 255 });
            DrawTexturePro(screen.texture, { 0.0f, 0.0f, (float)screen.texture.width, -(float)screen.texture.height }, { origin.x, origin.y, (float)windowWidth*2.0f, (float)windowHeight*2.0f }, origin, rotation, WHITE);
            if (player.isDead())
                DrawTexturePro(deadSprite, { 0.0f, 0.0f, 16.0f, 16.0f }, { 0.0f, 0.0f, 16.0f * scale, 16.0f * scale }, { 0.0f, 0.0f }, 0.0f, WHITE);
        EndDrawing();
    }
}

void Game::startTransition(float value)
{
    if (!onTransition)
    {
        this->transitionValue = value;
        onTransition = true;
        transitionTimer = 0.0f;
        targetRotation = rotation - value;
        player.setRotation(targetRotation);
        player.setDirection(value);
        player.setOnTransition(true);
        map.setRotation(targetRotation);
        if (value > 0)
            map.nextMap();
        else
            map.previousMap();
    }
}

void Game::endTransition()
{
    if (onTransition)
    {
        onTransition = false;
        // paused = false;
        rotation = targetRotation;
        oldRotation = rotation;
        player.setOnTransition(false);
        map.disablePrevious(transitionValue);
        // map.setOnTransition(false);
    }
}

float Game::easeInOutSine(float start, float end, float duration, float time) {
    float t = time / duration;
    return start + (end - start) * (-(std::cos(PI * t) - 1) / 2);
}

float Game::easeInCubic(float start, float end, float duration, float time)
{
    float t = time / duration;
    return start + (end - start) * (t * t * t);
}
