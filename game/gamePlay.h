#pragma once
#include "loadAssets.h"
#include "Poke.h"
#include "gameState.h"
#include <map>
#include <vector>
#include <random>
#include <queue>

bool isValid(int x, int y);

bool isReachable(mapObj** tempMap, int startX, int startY);
void generateMap(int enemyCount, int healCount, int difficulty);

void printMapToConsole();

void DrawGameplay();