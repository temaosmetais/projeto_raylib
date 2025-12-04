#ifndef MAP_H
#define MAP_H
#include "game_constants.h"
#include <stdio.h>
void InitMap(Map* map);
Vector2 LoadMapFromFile(Map* map, const char* filename);
void DrawMap(Map* map);
void UnloadMap(Map* map);
#endif
