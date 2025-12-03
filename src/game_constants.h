#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "raylib.h"

// --- MAPA ---
#define TILE_SIZE 50
#define MAP_ROWS 16
#define MAP_COLS 150
#define TILE_SOURCE_SIZE 16   // Tamanho do tile na imagem nestle.png
#define MAX_MONSTERS 10

// --- JOGADOR ---
#define GRAVITY 800.0f
#define JUMP_FORCE -550.0f
#define PLAYER_SPEED 300.0f
#define MAX_LIVES 5

typedef struct Player {
    Vector2 position;
    Vector2 speed;
    bool canJump;
    float width;
    float height;

    // Gráficos
    Texture2D runTextures[3];
    Texture2D jumpTexture;

    // Animação
    int frameCounter;
    int currentFrame;
    bool isFacingRight;

    // Vida e Dano
    int lives;
    float invincibilityTimer;
    bool isDead;
} Player;

typedef struct Monster {
    Vector2 position;
    Vector2 speed;
    float width;
    float height;
    bool active;
    float moveTimer; // Tempo para mudar de direção
    int direction;   // -1 ou 1
} Monster;

typedef struct Map {
    Rectangle barriers[MAP_ROWS * MAP_COLS];
    int barriersCount;
    char layout[MAP_ROWS][MAP_COLS + 1];

    Monster monsters[MAX_MONSTERS];
    int monstersCount;

    bool isVillage; // Se true, pode ter lógica de loja (futuro)

    Texture2D tilesetTexture;
    Texture2D backgroundTexture;
} Map;

#endif
