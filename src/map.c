#include "../include/raylib.h"
#include "map.h"

void ResetMap(Map* map) {
    map->barriersCount = 0;
    map->monstersCount = 0;
    map->isVillage = false;
}

void InitMap(Map* map) {
    // Carrega texturas globais do mapa
    map->tilesetTexture = LoadTexture("assets/nestle.png");
    map->backgroundTexture = LoadTexture("assets/fundo.png");
    ResetMap(map);
}

Vector2 LoadMapFromFile(Map* map, const char* filename) {
    ResetMap(map);
    FILE* file = fopen(filename, "r");
    Vector2 playerStart = {100, 300}; // Padrão se não achar 'J'

    if (!file) {
        printf("ERRO: Nao foi possivel abrir %s\n", filename);
        return playerStart;
    }

    char ch;
    int row = 0;
    int col = 0;

    while ((ch = fgetc(file)) != EOF && row < MAP_ROWS) {
        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }

        if (col < MAP_COLS) {
            map->layout[row][col] = ch;
            float px = col * TILE_SIZE;
            float py = row * TILE_SIZE;

            if (ch == 'P') { // Parede
                map->barriers[map->barriersCount] = (Rectangle){ px, py, TILE_SIZE, TILE_SIZE };
                map->barriersCount++;
            }
            else if (ch == 'J') { // Jogador
                playerStart = (Vector2){ px, py };
                map->layout[row][col] = ' '; // Limpa visualmente
            }
            else if (ch == 'M') { // Monstro
                if (map->monstersCount < MAX_MONSTERS) {
                    Monster* m = &map->monsters[map->monstersCount];
                    m->position = (Vector2){ px, py };
                    m->speed = (Vector2){0,0};
                    m->width = 40; m->height = 40;
                    m->active = true;
                    m->moveTimer = 0;
                    m->direction = 1;
                    map->monstersCount++;
                }
                map->layout[row][col] = ' ';
            }
            col++;
        }
    }
    fclose(file);
    return playerStart;
}

void DrawMap(Map* map) {
    // 1. Fundo Repetido
    int bgW = map->backgroundTexture.width;
    int bgH = map->backgroundTexture.height;

    if (bgW > 0) {
        // Cobre toda a extensão do mapa (150 blocos)
        for (int y = 0; y < MAP_ROWS * TILE_SIZE; y += bgH) {
            for (int x = 0; x < MAP_COLS * TILE_SIZE; x += bgW) {
                DrawTexture(map->backgroundTexture, x, y, WHITE);
            }
        }
    } else {
        ClearBackground(RAYWHITE);
    }

    // 2. Paredes
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            if (map->layout[y][x] == 'P') {
                // Desenha usando o tileset (ajuste o recorte se precisar)
                Rectangle src = { 0, 0, map->tilesetTexture.width, map->tilesetTexture.height }; // Imagem inteira ou recorte
                // Se usar nestle.png (tileset), mude o src acima para {0,0,16,16} ou similar
                Rectangle dst = { x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE };
                DrawTexturePro(map->tilesetTexture, src, dst, (Vector2){0,0}, 0.0f, WHITE);
            }
        }
    }
}

void UnloadMap(Map* map) {
    UnloadTexture(map->tilesetTexture);
    UnloadTexture(map->backgroundTexture);
}
