#include "drawMenus.h"
#include "loadAssets.h"
#include "gameState.h"
#include "gamePlay.h"
#include <string>
#include <iostream>

void DrawSettings(const float& x, const float& y) {
    Rectangle win = { x, y, 400, 500 };
    if (GuiWindowBox(win, "Settings")) showSettingsWindow = false;
    GuiSlider({ x + 100, y + 50, 200, 20 }, "Volume", nullptr, &musicVolume, 0.0f, 1.0f);
    GuiComboBox({ x + 20, y + 90, 200, 20 }, "Easy;Medium;Hard", &difficultyActive);
}

void DrawMenu(bool& togglePressed, int& scrollIndex, int& activeIndex, int& focus, const char** items, int itemCount) {

    DrawTexture(GetTexture("menu_main"), 0, 0, WHITE);

    if (IsKeyPressed(KEY_ESCAPE) || GuiButton({ 225, 590, 243, 80 }, "")) showSettingsWindow = !showSettingsWindow;

    if (!showSettingsWindow) {
        if (GuiButton({ 178, 463, 325, 100 }, "")) StartTransition(SCREEN_CHARACTER_SELECT);
        GuiToggle({ 146, 335, 389, 96 }, "", &togglePressed);
        if (togglePressed) GuiListViewEx({ 855, 200, 400, 500 }, items, itemCount, &scrollIndex, &activeIndex, &focus);
    }

    else {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 100 });
        DrawSettings(855, 200);
    }

}

void DrawCharacterSelect(char* name, bool& editMode, int& curr) {
    DrawTexture(GetTexture("menu_select_person"), 0, 0, WHITE);
    if (GuiButton({ 310, 844, 275, 110 }, "")) StartTransition(SCREEN_MENU);
    if (GuiTextBox({ 535, 92, 431, 88 }, name, 64, editMode)) editMode = !editMode;
    if (GuiButton({ 858, 844, 372, 110 }, "") && name[0] != '\0')
    {
        player.name = name;
        player.skins[0] = GetTexture("person_" + std::to_string(curr));
        player.skins[1] = GetTexture("person_f_" + std::to_string(curr));
        player.skins[2] = GetTexture("person_b_" + std::to_string(curr));
        player.skins[3] = GetTexture("person_r_" + std::to_string(curr));
        player.skins[4] = GetTexture("person_l_" + std::to_string(curr));
        StartTransition(SCREEN_POKEMONS_SELECT);
    }
    DrawTextureEx(GetTexture("person_" + std::to_string(curr)), { 500, 250 }, 0, 0.5f, WHITE);
    if (GuiButton({ 1260, 444, 100, 180 }, "")) curr = (curr + 1) % 4;
    if (GuiButton({ 168, 444, 100, 180 }, "")) curr = (curr + 3) % 4;
}

void DrawPokemonSelect(const std::map<std::string, Pokemon>& allPokes) {
    DrawTexture(GetTexture("menu_select_pokemons"), 0, 0, WHITE);

    // Кнопка "НАЗАД"
    if (GuiButton({ 272, 794, 272, 108 }, "")) StartTransition(SCREEN_CHARACTER_SELECT);

    // Переключатели сложности
    bool toggle0 = difficultyActive == 0;
    bool toggle1 = difficultyActive == 1;
    bool toggle2 = difficultyActive == 2;

    GuiToggle({ 202, 127, 300, 110 }, "", &toggle0);
    GuiToggle({ 571, 127, 384, 110 }, "", &toggle1);
    GuiToggle({ 1020, 127, 317, 110 }, "", &toggle2);

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, { 202, 127, 300, 110 })) difficultyActive = 0;
        else if (CheckCollisionPointRec(mouse, { 571, 127, 384, 110 })) difficultyActive = 1;
        else if (CheckCollisionPointRec(mouse, { 1020, 127, 317, 110 })) difficultyActive = 2;
    }

    // Покемоны: 2 ряда по 5, с твоими координатами и новым размером 167 x 144
    for (int i = 0; i < 10; i++) {
        int row = i / 5;
        int col = i % 5;

        Rectangle rect = {
            241 + col * (167 + 63),
            301 + row * (144 + 63),  // новый шаг по вертикали
            167, 144
        };

        // Картинка покемона
        DrawTextureEx(GetTexture("poke_" + std::to_string(i + 1)), { rect.x, (rect.y)-10 }, 0, 2.0f, WHITE);

        // Выбор
        bool prev = selectedPokemons[i];
        GuiToggle(rect, "", &selectedPokemons[i]);

        if (!prev && selectedPokemons[i]) {
            int count = 0;
            for (int j = 0; j < 10; j++) if (selectedPokemons[j]) count++;
            if (count > 3) selectedPokemons[i] = false;
        }
    }

    int selectedCount = 0;
    for (int i = 0; i < 10; i++) if (selectedPokemons[i]) selectedCount++;

    if (selectedCount == 3) {
        if (GuiButton({ 1027, 795, 276, 109 }, "")) {
            player.dificult = difficultyActive;
            int index = 0;
            for (int i = 0; i < 10 && index < 3; i++) {
                if (selectedPokemons[i]) {
                    std::string textureName = "poke_" + std::to_string(i + 1);

                    for (const auto& pair : allPokes) {
                        if (nameToTexture(pair.first) == textureName) {
                            player.team[index++] = pair.second;
                            std::cout << player.team[index - 1].getName();
                            break;
                        }
                    }   
                }
            }
            generateMap(5, 5, player.dificult);
            printMapToConsole();
            StartTransition(SCREEN_GAMEPLAY);
        }
    }
    else {
        GuiDisable();
        GuiButton({ 1027, 795, 276, 109 }, "");
        GuiEnable();
    }
}
