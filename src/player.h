#ifndef PLAYER_H
#define PLAYER_H
#include "game_constants.h"

void InitPlayer(Player* player);
void UpdatePlayer(Player* player, Map* map, float delta);
void DrawPlayer(Player* player);
void UnloadPlayer(Player* player);

extern bool playHitSound;
extern bool playJumpSound;

#endif
