#ifndef SCREENS_H
#define SCREENS_H
#include "raylib.h"
#include "utils.h"

typedef enum GameScreen { MENU=0, GAMEPLAY, PAUSE, LOADING, HELP, EXITING } GameScreen;

// (Funções de Menu, Loading, Help omitidas para brevidade - mantenha as antigas)
void InitMenu(Rectangle* a, Rectangle* b, Rectangle* c, Rectangle* d);
GameScreen UpdateMenu(GameScreen s, const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d);
void DrawMenu(const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d);

GameScreen UpdateGameplay(GameScreen currentScreen);
void DrawGameplay();

// Adicione os cabeçalhos para Pause, Help, etc aqui...
GameScreen UpdatePause(GameScreen s);
void DrawPause(GameScreen* s);
#endif
