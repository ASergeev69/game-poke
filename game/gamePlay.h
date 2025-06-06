#pragma once

#include "loadAssets.h"
#include "gameState.h"
#include "Poke.h"
#include <map>
#include <vector>
#include <random>
#include <queue>

enum class mapObjType
{
    WALL,
    ENEMY,
    PLAYER,
    HEAL,
    AIR
};

struct mapObj {
    mapObjType type;
    Texture2D texture;
    int x;
    int y;
};

bool isValid(int x, int y);

bool isReachable(mapObj** tempMap, int startX, int startY);
void generateMap(int enemyCount, int healCount, int difficulty, const std::map<std::string, Pokemon>& allPokes);

void printMapToConsole();

struct Player {
    std::string name;
    Pokemon team[3];
    int dificult;
    Texture2D skins[5];
    int exp = 0;
    int x = 5;
    int y = 5;

    bool teamIsAlive()
    {
        int countDie = 0;
        for (int i{}; i < 3; ++i)
            if (team[i].getHp() == 0)
                countDie += 1;

        return !(countDie == 3);
    }

    void walk(int step_x, int step_y);

    void gainHP();
};

extern Player player;

void DrawGameplay();