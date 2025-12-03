#include "../include/raylib.h"
#include "utils.h"
#include "screens.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Hollow Knight Clone");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = MENU;
    Rectangle btnPlay, btnLoad, btnHelp, btnExit;
    InitMenu(&btnPlay, &btnLoad, &btnHelp, &btnExit);

    while (!WindowShouldClose()) {
        switch(currentScreen) {
            case MENU: currentScreen = UpdateMenu(currentScreen, &btnPlay, &btnLoad, &btnHelp, &btnExit); break;
            case GAMEPLAY: currentScreen = UpdateGameplay(currentScreen); break;
            case PAUSE: currentScreen = UpdatePause(currentScreen); break;
            case EXITING: goto End;
            default: break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(currentScreen) {
            case MENU: DrawMenu(&btnPlay, &btnLoad, &btnHelp, &btnExit); break;
            case GAMEPLAY: DrawGameplay(); break;
            case PAUSE: DrawPause(&currentScreen); break;
            default: break;
        }
        EndDrawing();
    }

    End:
    CloseWindow();
    return 0;
}
