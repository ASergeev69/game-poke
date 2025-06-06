#pragma once
#include "Poke.h"

void DrawMenu(bool& togglePressed, int& scrollIndex, int& activeIndex, int& focus, const char** items, int itemCount);
void DrawCharacterSelect(char* name, bool& editMode, int& curr);
void DrawPokemonSelect(const std::map<std::string, Pokemon>& allPokes);
void DrawSettings(const float& x, const float& y);