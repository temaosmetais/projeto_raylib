#include "../include/raylib.h"
#include "monster.h"
#include <stdlib.h> // rand()

void UpdateMonsters(Map* map, float delta) {
    for (int i = 0; i < map->monstersCount; i++) {
        Monster* m = &map->monsters[i];
        if (!m->active) continue;

        // IA Simples: Muda de direção aleatoriamente
        m->moveTimer -= delta;
        if (m->moveTimer <= 0) {
            m->moveTimer = (float)(rand() % 3) + 1.0f;
            m->direction = (rand() % 3) - 1; // -1, 0, 1
        }

        // Movimento X
        float moveX = m->direction * 100.0f * delta; // Velocidade 100
        m->position.x += moveX;

        // Colisão X (Paredes)
        Rectangle mRect = { m->position.x, m->position.y, m->width, m->height };
        for (int j = 0; j < map->barriersCount; j++) {
            if (CheckCollisionRecs(mRect, map->barriers[j])) {
                if (moveX > 0) m->position.x = map->barriers[j].x - m->width;
                else if (moveX < 0) m->position.x = map->barriers[j].x + map->barriers[j].width;
                m->direction *= -1; // Bate e volta
            }
        }

        // Gravidade e Colisão Y
        m->speed.y += GRAVITY * delta;
        m->position.y += m->speed.y * delta;
        mRect.x = m->position.x;
        mRect.y = m->position.y;

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
    for (int i = 0; i < map->monstersCount; i++) {
        Monster* m = &map->monsters[i];
        if (m->active) {
            // Desenha um quadrado vermelho (monstro)
            DrawRectangle(m->position.x, m->position.y, m->width, m->height, RED);
        }
    }
}
