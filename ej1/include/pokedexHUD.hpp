#ifndef POKEDEXHUD_HPP
#define POKEDEXHUD_HPP

#include "libs.hpp"

inline string getPixelColor(int r, int g, int b) {
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m  \033[0m";
}

void clearScreen();

vector<unsigned char*> cargarImagenesPokemones(vector<Pokemon> pokemons, vector<int>& widths, vector<int>& heights, vector<int>& chans);

void printCentered(string& output, string text, int width);

string barCreator(float percentage);

int showOptions(size_t currentPage, size_t totalPages);

int emptysAndLeftTabs(string& output, vector<Pokemon> pokemons, int width);

void handleAddPokemon(Pokedex& pokedex);
void handleShowPokemon(Pokedex& pokedex);
void handlePageOrPokedexChange(Pokedex& pokedex, int option);

int askForPokedex(vector<string> savedPokedexes);

void loadingScreen();
#endif