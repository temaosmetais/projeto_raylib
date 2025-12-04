#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "../include/raylib.h"

// --- MAPA ---
#define TILE_SIZE 50
#define MAP_ROWS 16
#define MAP_COLS 150
#define MAX_MONSTERS 30
#define MAX_PORTALS 5

// --- JOGADOR ---
#define GRAVITY 800.0f
#define JUMP_FORCE -550.0f
#define PLAYER_SPEED 300.0f
#define MAX_LIVES 5
#define ATTACK_DURATION 0.3f
#define ATTACK_COOLDOWN 0.5f

// --- ESTRUTURAS ---

typedef struct Player {
    Vector2 position;
    Vector2 speed;
    bool canJump;
    float width;
    float height;
    
    // Gráficos
    Texture2D runTextures[3]; 
    Texture2D jumpTexture;
	Texture2D attackTexture;
    
    // Animação e Estado
    int frameCounter;
    int currentFrame;
    bool isFacingRight;

    // Vida
    int lives;
    float invincibilityTimer;
    bool isDead;

    // Combate
    bool isAttacking;
    float attackTimer;
    float cooldownTimer;
    Rectangle attackHitbox;
} Player;

typedef struct Monster {
    Vector2 position;
    Vector2 speed;
    float width;
    float height;
    bool active;
    float moveTimer;
    int direction; 
    int hp;
} Monster;

typedef struct Portal {
    Rectangle rect;
    bool active;
} Portal;

typedef struct Map {
    Rectangle barriers[MAP_ROWS * MAP_COLS];
    int barriersCount;
    char layout[MAP_ROWS][MAP_COLS + 1];
    
    Monster monsters[MAX_MONSTERS];
    int monstersCount;

    Portal portals[MAX_PORTALS];
    int portalsCount;

    bool isVillage;
    
    Texture2D tilesetTexture;
    Texture2D backgroundTexture;
	Texture2D monsterTextures[2];
    Texture2D portalTexture;
} Map;

#endif
