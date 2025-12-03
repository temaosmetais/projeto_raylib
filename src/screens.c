#include "screens.h"
#include "map.h"
#include "player.h"
#include "monster.h"
#include <stdio.h>

static Player player;
static Map currentMap;
static Camera2D camera = {0};
static bool gameInitialized = false;

// Controle de Fase
static int currentLevel = 0;
const char* levels[] = { "assets/vila.txt", "assets/tunel1.txt" };
const int maxLevels = 2;

// --- MENU ---
void InitMenu(Rectangle* a, Rectangle* b, Rectangle* c, Rectangle* d) {
    *a = (Rectangle){ screenWidth/2 - 150, 200, 300, 30 };
    *b = (Rectangle){ screenWidth/2 - 150, 250, 300, 30 };
    *c = (Rectangle){ screenWidth/2 - 150, 300, 300, 30 };
    *d = (Rectangle){ screenWidth/2 - 150, 350, 300, 30 };
}

GameScreen UpdateMenu(GameScreen s, const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d) {
    Vector2 m = GetMousePosition();
    if (CheckCollisionPointRec(m, *a) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return GAMEPLAY;
    if (CheckCollisionPointRec(m, *b) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return LOADING;
    if (CheckCollisionPointRec(m, *c) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return HELP;
    if (CheckCollisionPointRec(m, *d) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return EXITING;
    return s;
}

void DrawMenu(const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d) {
    DrawText("HOLLOW KNIGHT CLONE", screenWidth/2 - MeasureText("HOLLOW KNIGHT CLONE", 40)/2, 80, 40, BLACK);
    DrawText("Jogar", a->x, a->y, 30, GREEN);
    DrawText("Carregar", b->x, b->y, 30, BLUE);
    DrawText("Ajuda", c->x, c->y, 30, GOLD);
    DrawText("Sair", d->x, d->y, 30, RED);
}

// --- GAMEPLAY ---
void LoadLevel(int idx) {
    if (idx >= maxLevels) idx = 0; // Loop ou Fim
    currentLevel = idx;
    Vector2 start = LoadMapFromFile(&currentMap, levels[idx]);
    player.position = start;
    player.speed = (Vector2){0,0};
    currentMap.isVillage = (idx == 0);
}

GameScreen UpdateGameplay(GameScreen s) {
    if (!gameInitialized) {
        InitPlayer(&player);
        InitMap(&currentMap);
        LoadLevel(0);
        camera.zoom = 1.0f;
        gameInitialized = true;
    }

    if (player.isDead) {
        if (IsKeyPressed(KEY_ENTER)) {
            LoadLevel(0);
            player.lives = MAX_LIVES;
            player.isDead = false;
        }
        return s;
    }

    float dt = GetFrameTime();
    UpdatePlayer(&player, &currentMap, dt);
    UpdateMonsters(&currentMap, dt);

    // Passar de Fase
    if (player.position.x > (MAP_COLS * TILE_SIZE) - 100) {
        LoadLevel(currentLevel + 1);
    }

    // Camera
    camera.target.x = player.position.x + player.width/2;
    camera.target.y = screenHeight / 2.0f;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };

    if (camera.target.x < screenWidth/2) camera.target.x = screenWidth/2;
    float maxW = MAP_COLS * TILE_SIZE;
    if (camera.target.x > maxW - screenWidth/2) camera.target.x = maxW - screenWidth/2;

    if (IsKeyPressed(KEY_ESCAPE)) return PAUSE;
    return s;
}

void DrawGameplay() {
    BeginMode2D(camera);
        DrawMap(&currentMap);
        DrawMonsters(&currentMap);
        DrawPlayer(&player);
    EndMode2D();

    if (player.isDead) {
        DrawText("VOCE MORREU! ENTER para reiniciar", 300, 350, 40, RED);
    } else {
        DrawText(TextFormat("Vidas: %d", player.lives), 20, 20, 30, RED);
        if (currentMap.isVillage) DrawText("VILA", 20, 60, 20, GREEN);
    }
}

// --- PAUSE ---
GameScreen UpdatePause(GameScreen s) {
    if (IsKeyPressed(KEY_ESCAPE)) return GAMEPLAY;
    if (IsKeyPressed(KEY_BACKSPACE)) return MENU;
    return s;
}
void DrawPause(GameScreen* s) {
    DrawRectangle(0,0,screenWidth,screenHeight,Fade(BLACK, 0.5f));
    DrawText("PAUSADO", screenWidth/2 - 100, screenHeight/2, 40, WHITE);
}
