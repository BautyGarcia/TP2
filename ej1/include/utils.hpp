#ifndef UTILS_HPP
#define UTILS_HPP

#include "libs.hpp"

struct PokemonInfoMapped{
    string name;
    string type;
    string description;
    vector<pair<string, int>> attacks;
    vector<size_t> xpRemaining;
};

//voy a usar un unordered_map para tener la info de los pokemones en memoria en vez de leer el csv todo el tiempo (es mas rapido)
//sin esta adicion, aun usando multithreading, tardaba 20/25 segundos en cargar la pokedex All Unlocked. Ahora tarda 2 segundos
extern unordered_map<size_t, PokemonInfoMapped> pokemonDataBase;
extern unordered_map<string, size_t> standardNameToPokedexID;

//pide al usuario que ingrese un nombre de pokemon y lo agrega a la pokedex
void handleAddPokemon(Pokedex& pokedex);

//pide al usuario que ingrese un nombre de pokemon y lo muestra en la pokedex
void handleShowPokemon(Pokedex& pokedex, bool printData);

//maneja cuando hay que cambiar de pagina
void handlePage(Pokedex& pokedex, int option);

//muestra las pokedex guardadas y pide al usuario que elija una
int askForPokedex(vector<string> savedPokedexes);

//carga todos los datos de los CSV la unordered_map pokemonDataBase
void loadPokemonDataBase();

#endif