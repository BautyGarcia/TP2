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

        void addPokemon(const Pokemon& pokemon);
        void removePokemon(const string& name);
        void show() const;
        void show(const Pokemon poke) const;

        void saveToFile();
        void loadFromFile(const string& filename);
};

#endif