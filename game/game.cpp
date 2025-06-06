#include "loadAssets.h"
#include "gameState.h"
#include "drawMenus.h"
#include "gamePlay.h"
#include <iostream>

int main() {
    InitWindow(1536, 1024, "Poke III: Adventure Time");
    SetTargetFPS(45);
    SetExitKey(KEY_NULL);
    loadStyles();

    char name[64] = "";
    bool editMode = false, togglePressed = false;
    int scrollIndex = 0, activeIndex = -1, focus = -1, curr = 0;
    const char* items[] = {
        "Save 1", "Save 2", "Save 3", "Save 4", "Save 5", "Save 6",
        "Save 7", "Save 8", "Save 9", "Save 10", "Save 11", "Save 12",
        "Save 13", "Save 14", "Save 15", "Save 16", "Save 17", "Save 18",
        "Save 19", "Save 20", "Save 21"
    };
    int itemCount = sizeof(items) / sizeof(items[0]);

    LoadTexturesFromDirectory("resources");
    LoadTexturesFromDirectory("resources/persons");
    LoadTexturesFromDirectory("resources/pokemons");
    std::map<std::string, Pokemon> allPokes = LoadPokemonDatabase("./poke_base.txt");
    LoadTexturesFromDirectory("resources/persons_ingame");
    LoadTexturesFromDirectory("resources/map_assets");


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) {
        case SCREEN_MENU: DrawMenu(togglePressed, scrollIndex, activeIndex, focus, items, itemCount); break;
        case SCREEN_CHARACTER_SELECT: DrawCharacterSelect(name, editMode, curr); break;
        case SCREEN_POKEMONS_SELECT: DrawPokemonSelect(allPokes); break;
        case SCREEN_GAMEPLAY:
            DrawGameplay();
            break;
        }

        DrawTransition();
        EndDrawing();
    }

    UnloadAllTextures();
    CloseWindow();
    return 0;
}
