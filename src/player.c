#include "../include/raylib.h"
#include "player.h"

bool playHitSound = false;
bool playJumpSound = false;

void InitPlayer(Player* player) {
    player->position = (Vector2){ 100, 300 };
    player->speed = (Vector2){ 0, 0 };
    player->width = 40;
    player->height = 50;
    player->lives = MAX_LIVES;
    player->isDead = false;
    player->invincibilityTimer = 0;
    player->isAttacking = false;
    player->attackTimer = 0;
    player->cooldownTimer = 0;
    player->frameCounter = 0;
    player->currentFrame = 0;
    player->isFacingRight = true;

    player->runTextures[0] = LoadTexture("assets/run1.png");
    player->runTextures[1] = LoadTexture("assets/run2.png");
    player->runTextures[2] = LoadTexture("assets/run3.png");
    player->jumpTexture = LoadTexture("assets/jump.png");

    // --- CARREGA O SPRITE DE ATAQUE (shot-sword.png renomeado para attack.png) ---
    player->attackTexture = LoadTexture("assets/attack.png");
}

void UpdatePlayer(Player* player, Map* map, float delta) {
    if (player->isDead) return;

    float moveX = 0;
    if (IsKeyDown(KEY_LEFT)) { moveX = -PLAYER_SPEED; player->isFacingRight = false; }
    if (IsKeyDown(KEY_RIGHT)) { moveX = PLAYER_SPEED; player->isFacingRight = true; }

    if (IsKeyPressed(KEY_SPACE) && player->canJump) {
        player->speed.y = JUMP_FORCE;
        player->canJump = false;
        playJumpSound = true;
    }

    if (player->cooldownTimer > 0) player->cooldownTimer -= delta;
    if (player->isAttacking) {
        player->attackTimer -= delta;
        if (player->attackTimer <= 0) player->isAttacking = false;
    }
    if (IsKeyPressed(KEY_X) && player->cooldownTimer <= 0 && !player->isAttacking) {
        player->isAttacking = true;
        player->attackTimer = ATTACK_DURATION;
        player->cooldownTimer = ATTACK_COOLDOWN;
        float range = 60.0f;
        player->attackHitbox = (Rectangle){
            player->isFacingRight ? player->position.x + player->width : player->position.x - range,
            player->position.y, range, player->height
        };
    }

    if (player->isAttacking) {
        float range = 60.0f;
        player->attackHitbox.x = player->isFacingRight ? player->position.x + player->width : player->position.x - range;
        player->attackHitbox.y = player->position.y;
        for (int i=0; i<map->monstersCount; i++) {
            Monster* m = &map->monsters[i];
            if (m->active && CheckCollisionRecs(player->attackHitbox, (Rectangle){m->position.x, m->position.y, m->width, m->height})) {
                m->active = false;
                playHitSound = true;
            }
        }
    }

    player->position.x += moveX * delta;
    Rectangle pRect = { player->position.x, player->position.y, player->width, player->height };
    for (int i = 0; i < map->barriersCount; i++) {
        if (CheckCollisionRecs(pRect, map->barriers[i])) {
            if (moveX > 0) player->position.x = map->barriers[i].x - player->width;
            else if (moveX < 0) player->position.x = map->barriers[i].x + map->barriers[i].width;
        }
    }

    player->speed.y += GRAVITY * delta;
    player->position.y += player->speed.y * delta;
    pRect.x = player->position.x; pRect.y = player->position.y;
    player->canJump = false;

    for (int i = 0; i < map->barriersCount; i++) {
        if (CheckCollisionRecs(pRect, map->barriers[i])) {
            if (player->speed.y > 0) {
                player->position.y = map->barriers[i].y - player->height;
                player->speed.y = 0;
                player->canJump = true;
            } else if (player->speed.y < 0) {
                player->position.y = map->barriers[i].y + map->barriers[i].height;
                player->speed.y = 0;
            }
        }
    }

    if (player->invincibilityTimer > 0) player->invincibilityTimer -= delta;
    else {
        pRect.x = player->position.x; pRect.y = player->position.y;
        for (int i = 0; i < map->monstersCount; i++) {
            Monster* m = &map->monsters[i];
            if (m->active && CheckCollisionRecs(pRect, (Rectangle){m->position.x, m->position.y, m->width, m->height})) {
                player->lives--;
                player->invincibilityTimer = 2.0f;
                player->speed.y = -300;
                if (player->lives <= 0) player->isDead = true;
                playHitSound = true;
            }
        }
    }

    if (player->canJump && moveX != 0) {
        player->frameCounter++;
        if (player->frameCounter >= 8) {
            player->frameCounter = 0;
            player->currentFrame = (player->currentFrame + 1) % 3;
        }
    } else if (player->canJump) player->currentFrame = 0;
}

void DrawPlayer(Player* player) {
    if (player->isDead) return;
    if (player->invincibilityTimer > 0 && (int)(player->invincibilityTimer*10)%2==0) return;

    Texture2D tex = player->canJump ? player->runTextures[player->currentFrame] : player->jumpTexture;
    float w = (float)tex.width;
    float h = (float)tex.height;
    if (!player->isFacingRight) w = -w;

    Rectangle dest = { player->position.x - 10, player->position.y - 10, 60, 60 };
    DrawTexturePro(tex, (Rectangle){0,0,w,h}, dest, (Vector2){0,0}, 0.0f, WHITE);

    // --- DESENHA O ATAQUE ---
    if (player->isAttacking) {
        Texture2D atkTex = player->attackTexture;
        float atkW = (float)atkTex.width;
        if (!player->isFacingRight) atkW = -atkW;

        // Desenha o corte da espada na frente do jogador
        Rectangle src = {0, 0, atkW, (float)atkTex.height};

        // Ajuste de posição para o corte ficar bonito
        float offX = player->isFacingRight ? 30 : -50;
        Rectangle destAtk = { player->position.x + offX, player->position.y, 60, 40 };

        DrawTexturePro(atkTex, src, destAtk, (Vector2){0,0}, 0.0f, WHITE);
    }
}

void UnloadPlayer(Player* player) {
    for(int i=0; i<3; i++) UnloadTexture(player->runTextures[i]);
    UnloadTexture(player->jumpTexture);
    UnloadTexture(player->attackTexture);
}
