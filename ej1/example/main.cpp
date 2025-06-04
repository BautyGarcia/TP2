#include "../include/pokedex.hpp"

void clearScreen();
void clearScreen(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void createFullPokedex(Pokedex& pokedex);
void createFullPokedex(Pokedex& pokedex) {
    ifstream file("assets/data/pokemons.csv");
    if (!file.is_open()) {
        cout << "Error opening pokemons data file." << endl;
        return;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string name;

        //skipeo el numero de pokedex y saco el nombre
        getline(ss, name, ',');
        getline(ss, name, ',');

        //lo meto en la pokedex
        pokedex.addPokemon(Pokemon(name));
    }
}

int main(){
    Pokedex pokedex("pokedex");
    pokedex.addPokemon(Pokemon("Charmander"));
    pokedex.addPokemon(Pokemon("Pikachu"));
    pokedex.addPokemon(Pokemon("Squirtle"));
    pokedex.addPokemon(Pokemon("Eevee"));
    pokedex.addPokemon(Pokemon("Jigglypuff"));
    pokedex.show();

    return 0;
}