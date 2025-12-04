#include "../include/raylib.h"
#include "map.h"

// --- CORREÇÃO 1: Limpeza total da matriz ---
void ResetMap(Map* map) {
    map->barriersCount = 0;
    map->monstersCount = 0;
    map->portalsCount = 0;
    map->isVillage = false;

    // Limpa a matriz visual preenchendo com espaços vazios
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map->layout[y][x] = ' ';
        }
    }
}

void InitMap(Map* map) {
    map->tilesetTexture = LoadTexture("assets/frente.png");
    map->backgroundTexture = LoadTexture("assets/fundo.png");

	map->monsterTextures[0] = LoadTexture("assets/monster1.png");
    map->monsterTextures[1] = LoadTexture("assets/monster2.png");

	map->portalTexture = LoadTexture("assets/portal.png");

    ResetMap(map);
}

Vector2 LoadMapFromFile(Map* map, const char* filename) {
    ResetMap(map); // Limpa tudo antes de ler

    FILE* file = fopen(filename, "r");
    Vector2 playerStart = {100, 300};

    if (!file) {
        printf("ERRO AO ABRIR: %s\n", filename);
        return playerStart;
    }

    char ch;
    int row = 0, col = 0;

    while ((ch = fgetc(file)) != EOF && row < MAP_ROWS) {
        if (ch == '\n') { row++; col = 0; continue; }

        if (col < MAP_COLS) {
            map->layout[row][col] = ch; // Salva o caractere
            float px = col * TILE_SIZE;
            float py = row * TILE_SIZE;

            if (ch == 'P') {
                map->barriers[map->barriersCount++] = (Rectangle){ px, py, TILE_SIZE, TILE_SIZE };
            }
            else if (ch == 'J') {
                playerStart = (Vector2){ px, py };
                map->layout[row][col] = ' ';
            }
            else if (ch == 'M') {
                if (map->monstersCount < MAX_MONSTERS) {
                    Monster* m = &map->monsters[map->monstersCount++];
                    m->position = (Vector2){ px, py };
                    m->speed = (Vector2){0,0};
                    m->width = 40; m->height = 40;
                    m->active = true;
                    m->hp = 3;
                    m->direction = 1;
                }
                map->layout[row][col] = ' ';
            }
            else if (ch == 'D' || ch == 'C') {
                if (map->portalsCount < MAX_PORTALS) {
                    map->portals[map->portalsCount++] = (Portal){
                        .rect = { px, py, TILE_SIZE, TILE_SIZE * 2 },
                        .active = true
                    };
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
    // Fundo
    int bgW = map->backgroundTexture.width;
    int bgH = map->backgroundTexture.height;
    if (bgW > 0) {
        for (int y=0; y<MAP_ROWS*TILE_SIZE; y+=bgH)
            for (int x=0; x<MAP_COLS*TILE_SIZE; x+=bgW)
                DrawTexture(map->backgroundTexture, x, y, WHITE);
    } else ClearBackground(RAYWHITE);

    // Portais
    for (int i = 0; i < map->portalsCount; i++) {
        Texture2D tex = map->portalTexture;
        
        // Origem: A imagem inteira
        Rectangle src = { 0, 0, 15.0f, 27.0f };
        
        // Destino: O retângulo de colisão do portal (que já tem tamanho 50x100)
        Rectangle dest = map->portals[i].rect;

        DrawTexturePro(tex, src, dest, (Vector2){0,0}, 0.0f, WHITE);
    }
    // Paredes
    for (int y=0; y<MAP_ROWS; y++) {
        for (int x=0; x<MAP_COLS; x++) {
            if (map->layout[y][x] == 'P') {
                Rectangle src = { 0, 0, 48, 48 };
                Rectangle dst = { x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE };
                DrawTexturePro(map->tilesetTexture, src, dst, (Vector2){0,0}, 0.0f, WHITE);
            }
        }
    }
}

void UnloadMap(Map* map) {
    UnloadTexture(map->tilesetTexture);
    UnloadTexture(map->backgroundTexture);

	UnloadTexture(map->monsterTextures[0]);
    UnloadTexture(map->monsterTextures[1]);

	UnloadTexture(map->portalTexture);
}
