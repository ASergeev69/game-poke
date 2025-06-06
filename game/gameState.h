#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "Poke.h"

namespace fs = std::filesystem;

enum GameScreen { SCREEN_MENU, SCREEN_CHARACTER_SELECT, SCREEN_POKEMONS_SELECT, SCREEN_GAMEPLAY, BATTLE };

extern GameScreen currentScreen;
extern GameScreen nextScreen;

enum class mapTypes
{
    WALL,
    ENEMY,
    PLAYER,
    HEAL,
    AIR
};

struct mapObj {
    mapTypes type;
    std::string textureName;
    bool passable = true;
};

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
extern mapObj** map;

extern bool selectedPokemons[10];

extern bool transitioning;
extern bool showSettingsWindow;

extern float transitionAlpha;
extern float transitionSpeed;
extern float musicVolume;

extern int difficultyActive;
extern int difficultyChoice;

//struct save {
//	save* pNext;
//	mapObj** map;
//	Player player;
//
//	save(mapObj** map, Player player)
//	{
//		this->map = map;
//		this->player = player;
//		pNext = nullptr;
//	}
//};
//
//class savesList {
//private:
//	save* head;
//	save* tail;
//public:
//	savesList()
//	{
//		head = nullptr;
//		tail = nullptr;
//	}
//
//	bool isEmpty()
//	{
//		return head == nullptr;
//	}
//
//	void push_back(mapObj** map, Player player)
//	{
//		save* p = new save(map, player);
//
//		if (isEmpty())
//		{
//			head = p;
//			tail = p;
//		}
//		else
//		{
//			tail->pNext = p;
//			tail = p;
//		}
//	}
//
//	void saveGame(fs::path fileName)
//	{
//		std::ofstream file(fileName);
//
//		if (file)
//			std::cerr << "ERROR: Bad read!!!!";
//		else if (!file.is_open())
//			std::cerr << "ERROR: Bad file!!!";
//		else
//		{
//
//		}
//	}
//};

void StartTransition(GameScreen target);
void DrawTransition();
