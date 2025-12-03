#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"
#include "utils.h"

typedef enum GameScreen {
	MENU = 0,
	GAMEPLAY,
	PAUSE,
	LOADING,
	HELP,
	EXITING
} GameScreen;

void InitMenu(Rectangle* btnPlay, Rectangle* btnLoad, Rectangle* btnHelp,
			  Rectangle* btnExit);
GameScreen UpdateMenu(GameScreen currentScreen, const Rectangle* btnPlay, 
					  const Rectangle* btnLoad, const Rectangle* btnHelp,
					  const Rectangle* btnExit);
void DrawMenu(const Rectangle* btnPlay, const Rectangle* btnLoad,
			  const Rectangle* btnHelp, const Rectangle* btnExit);

GameScreen UpdateGameplay(GameScreen currentScreen);
void DrawGameplay();

GameScreen UpdatePause(GameScreen currentScreen);
void DrawPause(GameScreen* currentScreen);

GameScreen UpdateLoading(GameScreen currentScreen);
void DrawLoading();

GameScreen UpdateHelp(GameScreen currentScreen);
void DrawHelp();

#endif // SCREENS_H
