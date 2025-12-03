#include "raylib.h"
#include "utils.h"
#include "screens.h"

int main(void)
{
	InitWindow(screenWidth, screenHeight, "Hollow Knight");
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	GameScreen currentScreen = MENU;

	Rectangle btnPlay = {0};
	Rectangle btnLoad = {0};
	Rectangle btnHelp = {0};
	Rectangle btnExit = {0};

	InitMenu(&btnPlay, &btnLoad, &btnHelp, &btnExit);

	while (!WindowShouldClose())
	{
		switch (currentScreen)
		{
			case MENU:
				currentScreen = UpdateMenu(currentScreen, &btnPlay, &btnLoad, &btnHelp, &btnExit);
				break;
			case GAMEPLAY:
				currentScreen = UpdateGameplay(currentScreen);
				break;
			case PAUSE:
				currentScreen = UpdatePause(currentScreen);
				break;
			case LOADING:
				currentScreen = UpdateLoading(currentScreen);
				break;
			case HELP:
				currentScreen = UpdateHelp(currentScreen);
				break;
			case EXITING:
				goto CloseGame;
			default: break;
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		switch (currentScreen)
		{
			case MENU:
				DrawMenu(&btnPlay, &btnLoad, &btnHelp, &btnExit);
				break;
			case GAMEPLAY:
				DrawGameplay();
				break;
			case PAUSE:
				DrawPause(&currentScreen);
				break;
			case HELP:
				DrawHelp();
				break;
			default: break;
		}

		EndDrawing();
	}

	CloseGame:
		CloseWindow();

	return 0;
}
