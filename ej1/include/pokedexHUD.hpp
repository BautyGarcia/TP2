#ifndef POKEDEXHUD_HPP
#define POKEDEXHUD_HPP

#include "libs.hpp"

//genera un caracter ANSII que representa un pixel de color RGB
inline string getPixelColor(int r, int g, int b) {
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m  \033[0m";
}

//limpia la pantalla de la terminal
void clearScreen();

//le pasas 3 pokemones y te devuelve un vector con el codigo RGB de cada pixel de la imagen
//para eso se usa una libreria externa llamada stb_image.h que encontramos en internet (esta el link del repo en el readme)
vector<unsigned char*> cargarImagenesPokemones(vector<Pokemon> pokemons, vector<int>& widths, vector<int>& heights, vector<int>& chans);

//agrega al output un texto con espacios a los costados para centrarlo en un ancho determinado
void printCentered(string& output, string text, int width);

//crea una barra de caracteres ANSII que representa el porcentaje de experiencia del pokemon
string barCreator(float percentage);

//muestra las opciones del menu
int showOptions(size_t currentPage, size_t totalPages);

//esto es mas raro, la ultima fila de la pokedex puede tener uno o dos pokemones, quedaba feo si no los centraba. Eso hace
int emptysAndLeftTabs(string& output, vector<Pokemon> pokemons, int width);

//lo mas importante de todo el tp, muestra un loading... mientras se carga la pokedex
void loadingScreen();
#endif