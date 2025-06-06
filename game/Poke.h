#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "loadAssets.h"

enum class Type {
    Normal,
    Fire,
    Water,
    Electric,
    Poison,
    Bug,
    Flying
};


std::string nameToTexture(const std::string& name);

Type StringToType(const std::string& s);

std::string TypeToString(Type t);

struct Move {
    std::string name;
    int power;

    Move(const std::string& n = "Tackle", int p = 20)
        : name(n), power(p) {
    }
};

class Pokemon {
private:
    std::string name;
    Texture2D skin;
    Type type;
    int level = 1;
    int maxHp = 0;
    int currentHp = 0;
    int attack = 0;
    int defense = 0;
    std::vector<Move> moves;
    bool evolved = false;

public:
    Pokemon() = default;

    Pokemon(const std::string& name_, int hp, int atk, int def, const std::vector<Move>& m, Type type_)
        : name(name_), maxHp(hp), currentHp(hp), attack(atk), defense(def), moves(m), type(type_) {
        skin = GetTexture(nameToTexture(name_));
    }
    std::vector<Move> getMoves()
    {
        return moves;
    }
    void Heal() { currentHp = maxHp; }
    void TakeDamage(int dmg) { currentHp = std::max(0, currentHp - dmg); }

    std::string getName() const { return name; }
    Type getType() const { return type; }
    Texture2D getSkin() const { return skin; }
    int getHp() const { return currentHp; }
    int getMaxHp() const { return maxHp; }

    void setSkin(Texture2D tex) { skin = tex; }

    // --- Перегрузка чтения из потока ---
    friend std::istream& operator>>(std::istream& is, Pokemon& poke) {
        std::string typeStr;
        is >> poke.name >> typeStr >> poke.maxHp >> poke.attack >> poke.defense;

        poke.type = StringToType(typeStr);
        poke.currentHp = poke.maxHp;
        poke.level = 1;
        poke.moves = { Move("default", 1), Move("Super puper", 3)};
        poke.skin = GetTexture(nameToTexture(poke.name));
        return is;
    }

    // --- Перегрузка записи в поток ---
    friend std::ostream& operator<<(std::ostream& os, const Pokemon& poke) {
        os << poke.name << " " << TypeToString(poke.type) << " " << poke.maxHp << " "
            << poke.attack << " " << poke.defense;
        return os;
    }
};
void GenerateRandomTeam(Pokemon team[3], const std::map<std::string, Pokemon>& allPokes);
std::map<std::string, Pokemon> LoadPokemonDatabase(const std::string& path);