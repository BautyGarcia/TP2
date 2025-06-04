#ifndef POKEDEXHUD_HPP
#define POKEDEXHUD_HPP

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
#include <filesystem>
#include <regex>
using namespace std;

#include "pokemon.hpp"
#include "stb_image.h"

class Pokedex;

//funcion para agregar un pixel de algun color dado (chat 🙂)
inline string getPixelColor(int r, int g, int b) {
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m  \033[0m";
}

void clearScreen();

vector<unsigned char*> cargarImagenesPokemones(vector<Pokemon> pokemons, vector<int>& widths, vector<int>& heights, vector<int>& chans);

void printCentered(string text, int width);

string barCreator(float percentage);

int showOptions(size_t currentPage, size_t totalPages);

int emptysAndLeftTabs(vector<Pokemon> pokemons, int width);

void handleAddPokemon(Pokedex& pokedex);
void handleShowPokemon(Pokedex& pokedex);
void handlePageOrPokedexChange(Pokedex& pokedex, int option);



#endif