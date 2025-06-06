#ifndef UTILS_HPP
#define UTILS_HPP

#include "libs.hpp"

struct PokemonInfoMapped{
    size_t pokedexID;
    string type;
    string description;
    vector<pair<string, size_t>> attacks;
    vector<size_t> xpRemaining;
};

//voy a usar un unordered_map para tener la info de los pokemones en memoria en vez de leer el csv todo el tiempo (es mas rapido)
extern unordered_map<string, PokemonInfoMapped> pokemonDataBase;

void handleAddPokemon(Pokedex& pokedex);
void handleShowPokemon(Pokedex& pokedex);
void handlePage(Pokedex& pokedex, int option);
int askForPokedex(vector<string> savedPokedexes);
void loadPokemonDataBase();

#endif