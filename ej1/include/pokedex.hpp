#ifndef POKEDEX_HPP
#define POKEDEX_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <fstream>
#include <thread>
#include <chrono>
#include <set>
#include <random>
#include <sstream>
#include <unordered_map>
using namespace std;

#include "pokemon.hpp"
#include "info.hpp"

class Pokedex {
    private:
        unordered_map<Pokemon, PokemonInfo, PokeHash> pokes;

    public:
        Pokedex() = default;

        void addPokemon(const Pokemon& pokemon, const PokemonInfo& info);
        void removePokemon(const string& name);
        void displayPokemon() const;
        void show(const Pokemon poke) const;

        void saveToFile(const string& filename);
        void loadFromFile(const string& filename);
};








#endif