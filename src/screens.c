#include "screens.h"
#include "utils.h"
#include "utils.h"

void InitMenu(Rectangle *btnPlay, Rectangle *btnLoad,
			  Rectangle *btnHelp, Rectangle *btnExit)
{
	*btnPlay = (Rectangle){ screenWidth / 2 - 150, 200, 300, 30 };
	*btnLoad = (Rectangle){ screenWidth / 2 - 150, 250, 300, 30 };
	*btnHelp = (Rectangle){ screenWidth / 2 - 150, 300, 300, 30 };
	*btnExit = (Rectangle){ screenWidth / 2 - 150, 350, 300, 30 };
}

GameScreen UpdateMenu(GameScreen currentScreen, const Rectangle *btnPlay,
					  const Rectangle *btnLoad, const Rectangle *btnHelp,
					  const Rectangle *btnExit)
{
	Vector2 mousePoint = GetMousePosition();

	if (CheckCollisionPointRec(mousePoint, *btnPlay) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		return GAMEPLAY;

	if (CheckCollisionPointRec(mousePoint, *btnLoad) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		return LOADING;

	if (CheckCollisionPointRec(mousePoint, *btnHelp) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		return HELP;

	if (CheckCollisionPointRec(mousePoint, *btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		return EXITING;

	return currentScreen;
}

void DrawMenu(const Rectangle *btnPlay, const Rectangle *btnLoad,
			  const Rectangle *btnHelp, const Rectangle *btnExit)
{
	Vector2 mousePoint = GetMousePosition();

	const char* titleText = "Hollow Knight";
	const int titleSize = 40;

	DrawText(titleText,
			 screenWidth / 2 - MeasureText(titleText, titleSize) / 2, 80, titleSize, BLACK);

	Color colorPlay = CheckCollisionPointRec(mousePoint, *btnPlay) ? DARKGREEN : GREEN;
	DrawText("Jogar (Novo Jogo)", btnPlay->x, btnPlay->y, 30, colorPlay);

	Color colorLoad = CheckCollisionPointRec(mousePoint, *btnLoad) ? DARKBLUE : BLUE;
	DrawText("Carregar Jogo", btnLoad->x, btnLoad->y, 30, colorLoad);

	Color colorHelp = CheckCollisionPointRec(mousePoint, *btnHelp) ? DARKBROWN : BROWN;
	DrawText("Ajuda (Instruções)", btnHelp->x, btnHelp->y, 30, colorHelp);

	Color colorExit = CheckCollisionPointRec(mousePoint, *btnExit) ? MAROON : RED;
	DrawText("Sair", btnExit->x, btnExit->y, 30, colorExit);
}

GameScreen UpdateGameplay(GameScreen currentScreen)
{
	if (IsKeyPressed(KEY_ESCAPE))
		return PAUSE;

	return currentScreen;
}

void DrawGameplay()
{
}

GameScreen UpdatePause(GameScreen currentScreen)
{
	if (IsKeyPressed(KEY_ESCAPE))
		return GAMEPLAY;

	return currentScreen;
}

void DrawPause(GameScreen* currentScreen)
{
	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

	const char* titleText = "JOGO PAUSADO";
	const char* resumeText = "Pressione ESC para Continuar";
	const char* menuText = "Pressione BACKSPACE ou M para voltar ao MENU";

	const int titleSize = 60;
	const int resumeSize = 30;
	const int menuSize = 20;

	int titleX = screenWidth / 2 - MeasureText(titleText, titleSize) / 2;
	DrawText(titleText, titleX, screenHeight / 2 - 50, titleSize, WHITE);

	int resumeX = screenWidth / 2 - MeasureText(resumeText, resumeSize) / 2;
	DrawText(resumeText, resumeX, screenHeight / 2 + 50, resumeSize, LIGHTGRAY);

	int menuX = screenWidth / 2 - MeasureText(menuText, menuSize) / 2;
	DrawText(menuText, menuX, screenHeight / 2 + 100, menuSize, LIGHTGRAY);

	if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_M))
		*currentScreen = MENU;
}

GameScreen UpdateLoading(GameScreen currentScreen)
{
	if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
		return MENU;

	if (IsKeyPressed(KEY_SPACE))
		return GAMEPLAY;

	return currentScreen;
}

void DrawLoading()
{
	const char* loadingText = "CARREGANDO JOGO ANTERIOR...";
	const char* returnText = "Pressione ESC ou BACKSPACE para voltar ao MENU";
	const char* startText = "Pressione ESPAÇO para iniciar o jogo carregado";

	const int loadingSize = 30;
	const int smallSize = 20;

	int loadingX = screenWidth / 2 - MeasureText(loadingText, loadingSize) / 2;
	DrawText(loadingText, loadingX, screenHeight / 2 - 50, loadingSize, BLUE);

	int returnX = screenWidth / 2 - MeasureText(returnText, smallSize) / 2;
	DrawText(returnText, returnX, screenHeight / 2 - 50, smallSize, DARKGREEN);

	int startX = screenWidth / 2 - MeasureText(startText, smallSize) / 2;
	DrawText(startText, startX, screenHeight / 2 - 50, smallSize, RED);
}

GameScreen UpdateHelp(GameScreen currentScreen)
{
	if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
		return MENU;

	return currentScreen;
}

void DrawHelp()
{
	const char* titleText = "AJUDA / INSTRUÇÕES";
	const char* controlsText = "Controles:";
	const char* moveText = "- Esquerda/Direita: Movimento";
	const char* jumpText = "- Espaço: Pulo";
	const char* returnText = "Pressione ESC ou BACKSPACE para voltar ao MENU";

	const int titleSize = 40;
	const int headerSize = 30;
	const int bodySize = 20;

	int titleX = screenWidth / 2 - MeasureText(titleText, titleSize) / 2;
	DrawText(titleText, titleX, 80, titleSize, BLACK);

	DrawText(controlsText, 100, 200, headerSize, DARKGRAY);
	DrawText(moveText, 100, 240, bodySize, DARKGRAY);
	DrawText(jumpText, 100, 270, bodySize, DARKGRAY);

	DrawText(returnText, 100, screenHeight - 50, bodySize, RED);
}
