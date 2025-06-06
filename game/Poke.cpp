#include "Poke.h"

std::map<std::string, Pokemon> LoadPokemonDatabase(const std::string& path) {
    std::ifstream file(path);
    std::map<std::string, Pokemon> database;

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл покемонов: " << path << std::endl;
        return database;
    }

    while (!file.eof()) {
        Pokemon p;
        file >> p;

        if (!p.getName().empty()) {
            database[p.getName()] = p;
        }
    }

    return database;
}

std::string TypeToString(Type t) {
    switch (t) {
    case Type::Fire: return "Fire";
    case Type::Water: return "Water";
    case Type::Electric: return "Electric";
    case Type::Poison: return "Poison";
    case Type::Bug: return "Bug";
    case Type::Flying: return "Flying";
    default: return "Normal";
    }
}

std::string nameToTexture(const std::string& name) {
    if (name == "bulbasaur")  return "poke_1";
    if (name == "caterpie")   return "poke_2";
    if (name == "charmander") return "poke_3";
    if (name == "eevee")      return "poke_4";
    if (name == "jigglypuff") return "poke_5";
    if (name == "pidgey")     return "poke_6";
    if (name == "pikachu")    return "poke_7";
    if (name == "rattata")    return "poke_8";
    if (name == "spearow")    return "poke_9";
    if (name == "squirtle")   return "poke_10";
    return "poke_7";
}

Type StringToType(const std::string& s) {
    if (s == "Fire") return Type::Fire;
    if (s == "Water") return Type::Water;
    if (s == "Electric") return Type::Electric;
    if (s == "Poison") return Type::Poison;
    if (s == "Bug") return Type::Bug;
    if (s == "Flying") return Type::Flying;
    return Type::Normal;
}