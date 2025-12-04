#include "../include/raylib.h"
#include "monster.h"
#include <stdlib.h>

void UpdateMonsters(Map* map, float delta) {
    for (int i = 0; i < map->monstersCount; i++) {
        Monster* m = &map->monsters[i];
        if (!m->active) continue;

        // IA Simples
        m->moveTimer -= delta;
        if (m->moveTimer <= 0) {
            m->moveTimer = (float)(rand() % 3) + 1.0f;
            m->direction = (rand() % 3) - 1; 
        }

        // Movimento X
        float moveX = m->direction * 100.0f * delta;
        m->position.x += moveX;

        // Colisão X
        Rectangle mRect = { m->position.x, m->position.y, m->width, m->height };
        for (int j = 0; j < map->barriersCount; j++) {
            if (CheckCollisionRecs(mRect, map->barriers[j])) {
                if (moveX > 0) m->position.x = map->barriers[j].x - m->width;
                else if (moveX < 0) m->position.x = map->barriers[j].x + map->barriers[j].width;
                m->direction *= -1;
            }
        }

        // Gravidade Y
        m->speed.y += GRAVITY * delta;
        m->position.y += m->speed.y * delta;
        mRect.x = m->position.x; mRect.y = m->position.y;

        // Colisão Y
        for (int j = 0; j < map->barriersCount; j++) {
            if (CheckCollisionRecs(mRect, map->barriers[j])) {
                 if (m->speed.y > 0) {
                    m->position.y = map->barriers[j].y - m->height;
                    m->speed.y = 0;
                 }
            }
        }
    }
}

void DrawMonsters(Map* map) {
    // --- CÁLCULO DO FRAME DE ANIMAÇÃO ---
    // GetTime() retorna o tempo total. Multiplicamos por 4 para velocidade (4 fps).
    // O operador % 2 faz alternar entre 0 e 1.
    int animFrame = (int)(GetTime() * 4.0f) % 2;

    for (int i = 0; i < map->monstersCount; i++) {
        Monster* m = &map->monsters[i];
        if (m->active) {

            // Seleciona a textura 0 ou 1 baseada no tempo
            Texture2D tex = map->monsterTextures[animFrame];

            float srcW = 20.0f;

            // Espelha se estiver indo para esquerda
            if (m->direction == -1) srcW = -srcW;

            // Desenha
            Rectangle src = { 0, 0, 20.0f, 16.0f };
            Rectangle dest = { m->position.x, m->position.y, m->width, m->height };

            DrawTexturePro(tex, src, dest, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}
