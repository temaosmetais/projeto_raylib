#ifndef PLAYER_H
#define PLAYER_H
#include "game_constants.h"
void InitPlayer(Player* player);
void UpdatePlayer(Player* player, Map* map, float delta);
void DrawPlayer(Player* player);
void UnloadPlayer(Player* player);

// Flags de Audio Globais
extern bool playHitSound;
extern bool playJumpSound;
#endif
