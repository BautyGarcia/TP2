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

#include "info.hpp"
#include "pokedexHUD.hpp"

class Pokedex {
    private:
        size_t currentPage = 1;
        string saveName;
        unordered_map<Pokemon, PokemonInfo, PokeHash> pokes;


        int getPokemonLevel(const pair<Pokemon, PokemonInfo>& entry) const;
        string pokemonImagesRow(vector<Pokemon> pokemons, int withOfPrint) const;
        void printThreePokes(vector<Pokemon> pokemons) const;
        void printNames(const vector<Pokemon> pokemons) const;
        void printXP(const vector<Pokemon> pokemons) const;
        void printAttacks(const vector<Pokemon> pokemons) const;

        vector<string> getDataToPrint(pair<Pokemon, PokemonInfo> entry) const;
    public:
        Pokedex(string name);

        string getSaveName() const { return saveName; }

        size_t getTotalPages() const;
        size_t getCurrentPage() const;
        void nextPage() { if (currentPage < getTotalPages()) currentPage++; }
        void previousPage() { if (currentPage > 1) currentPage--; }

        void addPokemon(const Pokemon& pokemon);
        void show() const;
        void show(const Pokemon poke) const;

        void saveToFile();
        void loadFromFile(const string& filename);
};

#endif