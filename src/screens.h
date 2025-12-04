#ifndef SCREENS_H
#define SCREENS_H
#include "raylib.h"
#include "utils.h"

typedef enum GameScreen { MENU=0, GAMEPLAY, PAUSE, LOADING, HELP, ENDING, EXITING } GameScreen;

void InitMenu(Rectangle* a, Rectangle* b, Rectangle* c, Rectangle* d);
GameScreen UpdateMenu(GameScreen s, const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d);
void DrawMenu(const Rectangle* a, const Rectangle* b, const Rectangle* c, const Rectangle* d);

GameScreen UpdateGameplay(GameScreen s);
void DrawGameplay();

GameScreen UpdateLoading(GameScreen s);
void DrawLoading();

GameScreen UpdateHelp(GameScreen s);
void DrawHelp();

GameScreen UpdatePause(GameScreen s);
void DrawPause(GameScreen* s);

GameScreen UpdateEnding(GameScreen s);
void DrawEnding();

#endif
