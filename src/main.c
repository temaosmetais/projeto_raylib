#include "raylib.h"
#include "utils.h"
#include "screens.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Hollow Knight Clone");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameScreen currentScreen = MENU;
    Rectangle a, b, c, d;
    InitMenu(&a, &b, &c, &d);

    while (!WindowShouldClose()) {
        switch(currentScreen) {
            case MENU: currentScreen = UpdateMenu(currentScreen, &a, &b, &c, &d); break;
            case GAMEPLAY: currentScreen = UpdateGameplay(currentScreen); break;
            case ENDING: currentScreen = UpdateEnding(currentScreen); break;
            case LOADING: currentScreen = UpdateLoading(currentScreen); break;
            case HELP: currentScreen = UpdateHelp(currentScreen); break;
            case PAUSE: currentScreen = UpdatePause(currentScreen); break;
            case EXITING: goto End;
            default: break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(currentScreen) {
            case MENU: DrawMenu(&a, &b, &c, &d); break;
            case GAMEPLAY: DrawGameplay(); break;
            case ENDING: DrawEnding(); break;
            case LOADING: DrawLoading(); break;
            case HELP: DrawHelp(); break;
            case PAUSE: DrawPause(&currentScreen); break;
            default: break;
        }
        EndDrawing();
    }

    End:
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
