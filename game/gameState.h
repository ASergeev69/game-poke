#pragma once
#include <map>       // ✅ для std::map
#include <string>    // ✅ для std::string
#include <vector>
#include "gamePlay.h"
#include "Poke.h"    // ✅ чтобы был известен тип Pokemon

enum GameScreen { SCREEN_MENU, SCREEN_CHARACTER_SELECT, SCREEN_POKEMONS_SELECT, SCREEN_GAMEPLAY, BATTLE };

extern GameScreen currentScreen;
extern GameScreen nextScreen;

extern Player player;
extern mapObj** map = nullptr;

extern bool selectedPokemons[10];

extern bool transitioning;
extern bool showSettingsWindow;

extern float transitionAlpha;
extern float transitionSpeed;
extern float musicVolume;

extern int difficultyActive;
extern int difficultyChoice;

void StartTransition(GameScreen target);
void DrawTransition();
