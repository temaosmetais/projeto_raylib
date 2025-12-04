#include "screens.h"
#include "map.h"
#include "player.h"
#include "monster.h"
#include <stdio.h>

static Player player;
static Map currentMap;
static Camera2D camera = {0};
static bool gameInitialized = false;

static bool startAsNewGame = true;
static int nextLevelIndex = 1;

static Music musicMenu, musicVila, musicTunel, musicGameOver, musicEnding;
static bool menuAudioLoaded = false;
static Sound soundHit, soundJump;

extern bool playHitSound;
extern bool playJumpSound;

static int currentLevel = 0;
const char* levels[] = {
    "assets/vila.txt",
    "assets/tunel1.txt",
    "assets/tunel2.txt",
    "assets/tunel3.txt"
};
const int maxLevels = 4;

// --- LEVEL TRANSITION LOGIC ---

void TransitionToLevel(int idx) {
    if (idx >= maxLevels) idx = 0;
    currentLevel = idx;

    StopMusicStream(musicVila);
    StopMusicStream(musicTunel);
    StopMusicStream(musicGameOver);
    StopMusicStream(musicEnding);

    Vector2 start = LoadMapFromFile(&currentMap, levels[idx]);
    player.position = start;
    player.speed = (Vector2){0,0};

    currentMap.isVillage = (idx == 0);

    if (currentMap.isVillage) PlayMusicStream(musicVila);
    else PlayMusicStream(musicTunel);
}

// ====================== GAMEPLAY ======================

GameScreen UpdateGameplay(GameScreen s) {
    if (!gameInitialized) {
        InitPlayer(&player);
        InitMap(&currentMap);
        if (!IsAudioDeviceReady()) InitAudioDevice();

        if (musicVila.stream.buffer == NULL) {
            musicVila = LoadMusicStream("assets/music_vila.mp3");
            musicTunel = LoadMusicStream("assets/music_tunel.mp3");
            musicGameOver = LoadMusicStream("assets/music_gameover.mp3");
            musicEnding = LoadMusicStream("assets/music_ending.mp3");

            soundHit = LoadSound("assets/hit.wav");
            soundJump = LoadSound("assets/jump.mp3");

            SetMusicVolume(musicVila, 0.5f);
            SetMusicVolume(musicTunel, 0.5f);
            SetMusicVolume(musicGameOver, 0.7f);
            SetMusicVolume(musicEnding, 0.6f);
        }

        if (startAsNewGame) {
            player.lives = MAX_LIVES;
            nextLevelIndex = 1;
            TransitionToLevel(0);
        } else {
            if (currentLevel >= maxLevels) currentLevel = maxLevels - 1;
            TransitionToLevel(currentLevel);
        }

        camera.zoom = 1.5f;
        gameInitialized = true;
    }

    // Audio Triggers
    if (playHitSound) { PlaySound(soundHit); playHitSound = false; }
    if (playJumpSound) { PlaySound(soundJump); playJumpSound = false; }

    // Death State
    if (player.isDead) {
        if (!IsMusicStreamPlaying(musicGameOver)) {
            StopMusicStream(musicVila);
            StopMusicStream(musicTunel);
            PlayMusicStream(musicGameOver);
        }
        UpdateMusicStream(musicGameOver);

        if (IsKeyPressed(KEY_ENTER)) {
            player.lives = MAX_LIVES;
            player.isDead = false;
            TransitionToLevel(0);
        }
        return s;
    }

    // Music State
    if (IsMusicStreamPlaying(musicGameOver)) StopMusicStream(musicGameOver);

    if (currentMap.isVillage) UpdateMusicStream(musicVila);
    else UpdateMusicStream(musicTunel);

    // Updates
    float dt = GetFrameTime();
    UpdatePlayer(&player, &currentMap, dt);
    UpdateMonsters(&currentMap, dt);

    // Cheat: Right boundary transition
    if (player.position.x > (MAP_COLS * TILE_SIZE) - 100) {
        if(currentMap.isVillage) TransitionToLevel(nextLevelIndex);
    }

    // Portal Logic
    Rectangle pRect = {player.position.x, player.position.y, player.width, player.height};
    for (int i = 0; i < currentMap.portalsCount; i++) {
        if (CheckCollisionRecs(pRect, currentMap.portals[i].rect)) {

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                if (currentMap.isVillage) {
                    TransitionToLevel(nextLevelIndex);
                } else {
                    if (currentLevel == maxLevels - 1) {
                        StopMusicStream(musicTunel);
                        return ENDING;
                    }
                    nextLevelIndex++;
                    if (nextLevelIndex >= maxLevels) nextLevelIndex = maxLevels - 1;
                    TransitionToLevel(0);
                }
            }
        }
    }

    // Camera Logic
    camera.target.x = player.position.x + player.width/2;
    camera.target.y = player.position.y + player.height/2;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };

    float hw = (screenWidth/2.0f)/camera.zoom;
    float hh = (screenHeight/2.0f)/camera.zoom;
    float mw = MAP_COLS*TILE_SIZE;
    float mh = MAP_ROWS*TILE_SIZE;

    if (camera.target.x < hw) camera.target.x = hw;
    if (camera.target.x > mw - hw) camera.target.x = mw - hw;
    if (camera.target.y < hh) camera.target.y = hh;
    if (camera.target.y > mh - hh) camera.target.y = mh - hh;

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
        DrawRectangle(0,0,screenWidth,screenHeight,Fade(BLACK,0.8f));

        const char* msgDead = "VOCÊ MORREU";
        const char* msgRest = "Aperte ENTER para renascer na vila";
        DrawText(msgDead, screenWidth/2 - MeasureText(msgDead, 40)/2, screenHeight/2 - 50, 40, RED);
        DrawText(msgRest, screenWidth/2 - MeasureText(msgRest, 20)/2, screenHeight/2 + 20, 20, WHITE);
    } else {
        const char* txtLives = TextFormat("VIDAS: %d", player.lives);
        DrawText(txtLives, 20, 20, 30, RED);

        if (currentMap.isVillage) {
            DrawText("VILA", 20, 60, 20, GREEN);
            DrawText(TextFormat("Proxima: Fase %d", nextLevelIndex), 20, 85, 20, LIME);
        } else {
            DrawText(TextFormat("FASE %d", currentLevel), 20, 60, 20, YELLOW);
        }
    }
}

// ====================== ENDING ======================

GameScreen UpdateEnding(GameScreen s) {
    if (!IsMusicStreamPlaying(musicEnding)) PlayMusicStream(musicEnding);
    UpdateMusicStream(musicEnding);

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        gameInitialized = false;
        StopMusicStream(musicVila);
        StopMusicStream(musicTunel);
        StopMusicStream(musicEnding);
        return MENU;
    }
    return s;
}

void DrawEnding() {
    ClearBackground(BLACK);
    const char* t1 = "FIM DE JOGO";
    const char* t2 = "Parabéns, Você completou a aventura!";
    const char* t3 = "Aperte ENTER para voltar ao MENU";

    DrawText(t1, screenWidth/2 - MeasureText(t1,60)/2, screenHeight/2 - 50, 60, GOLD);
    DrawText(t2, screenWidth/2 - MeasureText(t2,20)/2, screenHeight/2 + 10, 20, WHITE);
    DrawText(t3, screenWidth/2 - MeasureText(t3,20)/2, screenHeight/2 + 60, 20, GRAY);
}

// ====================== MENU ======================

void InitMenu(Rectangle* a, Rectangle* b, Rectangle* c, Rectangle* d) {
    float cx = screenWidth/2.0f - 150;
    *a = (Rectangle){cx, 200, 300, 30};
    *b = (Rectangle){cx, 250, 300, 30};
    *c = (Rectangle){cx, 300, 300, 30};
    *d = (Rectangle){cx, 350, 300, 30};
}

GameScreen UpdateMenu(GameScreen s, const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d) {
    if (!IsAudioDeviceReady()) InitAudioDevice();

    if (!menuAudioLoaded) {
        musicMenu = LoadMusicStream("assets/music_menu.mp3");
        SetMusicVolume(musicMenu, 0.5f);
        PlayMusicStream(musicMenu);
        menuAudioLoaded = true;
    }

    UpdateMusicStream(musicMenu);
    if (!IsMusicStreamPlaying(musicMenu)) PlayMusicStream(musicMenu);

    if (gameInitialized) {
        if(IsMusicStreamPlaying(musicVila)) StopMusicStream(musicVila);
        if(IsMusicStreamPlaying(musicTunel)) StopMusicStream(musicTunel);
    }

    Vector2 m = GetMousePosition();

    if (CheckCollisionPointRec(m, *a) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        startAsNewGame = true;
        StopMusicStream(musicMenu);
        return GAMEPLAY;
    }

    if (CheckCollisionPointRec(m, *b) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        startAsNewGame = false;
        StopMusicStream(musicMenu);
        return GAMEPLAY;
    }

    if (CheckCollisionPointRec(m, *c) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return HELP;
    if (CheckCollisionPointRec(m, *d) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return EXITING;

    return s;
}

void DrawMenu(const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d) {
    const char* title = "HOLLOW KNIGHT CLONE";
    DrawText(title, screenWidth/2 - MeasureText(title, 40)/2, 80, 40, BLACK);

    Vector2 m = GetMousePosition();
    DrawText("Jogar (Novo)", a->x, a->y, 30, CheckCollisionPointRec(m,*a) ? DARKGREEN : GREEN);
    DrawText("Carregar", b->x, b->y, 30, CheckCollisionPointRec(m,*b) ? DARKBLUE : BLUE);
    DrawText("Ajuda", c->x, c->y, 30, CheckCollisionPointRec(m,*c) ? DARKBROWN : GOLD);
    DrawText("Sair", d->x, d->y, 30, CheckCollisionPointRec(m,*d) ? MAROON : RED);
}

// ====================== EXTRAS ======================

GameScreen UpdatePause(GameScreen s) {
    if (IsKeyPressed(KEY_ESCAPE)) return GAMEPLAY;
    if (IsKeyPressed(KEY_BACKSPACE)) {
        gameInitialized = false;
        StopMusicStream(musicVila);
        StopMusicStream(musicTunel);
        return MENU;
    }
    return s;
}

void DrawPause(GameScreen* s) {
    DrawRectangle(0,0,screenWidth,screenHeight,Fade(BLACK,0.5f));
    const char* t1 = "JOGO PAUSADO";
    const char* t2 = "Aperte ESC para retomar jogo ou BACKSPACE para voltar ao MENU";
    DrawText(t1, screenWidth/2 - MeasureText(t1,40)/2, screenHeight/2 - 20, 40, WHITE);
    DrawText(t2, screenWidth/2 - MeasureText(t2,20)/2, screenHeight/2 + 30, 20, LIGHTGRAY);
}

GameScreen UpdateLoading(GameScreen s) {
    UpdateMusicStream(musicMenu);
    if (IsKeyPressed(KEY_ESCAPE)) return MENU;
    if (IsKeyPressed(KEY_SPACE)) {
        StopMusicStream(musicMenu);
        return GAMEPLAY;
    }
    return s;
}

void DrawLoading() {
    const char* t1 = "CARREGANDO...";
    DrawText(t1, 100, 100, 30, BLUE);
}

GameScreen UpdateHelp(GameScreen s) {
    UpdateMusicStream(musicMenu);
    if (IsKeyPressed(KEY_ESCAPE)) return MENU;
    return s;
}

void DrawHelp() {
    DrawText("AJUDA", screenWidth/2 - MeasureText("AJUDA", 40)/2, 50, 40, BLACK);

    int y = 150;
    DrawText("Setas: Mover", 100, y, 20, DARKGRAY);
    DrawText("Espaço: Pular", 100, y+30, 20, DARKGRAY);
    DrawText("X: Atacar", 100, y+60, 20, DARKGRAY);
    DrawText("SETA PARA CIMA: Entrar no portal/portão", 100, y+90, 20, DARKGRAY);
    DrawText("ESC: Voltar para MENU", 100, screenHeight - 50, 20, RED);
}
