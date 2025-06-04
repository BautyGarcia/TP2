#include "../include/pokedex.hpp"

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
    Pokedex pokedex("Full Pokedex Test");
    createFullPokedex(pokedex);
    
    while (true){
        clearScreen();
        pokedex.show();
        cout << endl << endl;
        int numOptions = showOptions(pokedex.getCurrentPage(), pokedex.getTotalPages());

        int option;
        cin >> option;
        while (!cin.good() || option < 0 || option > numOptions - 1) {
            cout << "Invalid input. Select a valid option.";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            this_thread::sleep_for(chrono::seconds(1));

            clearScreen();
            pokedex.show();
            cout << endl << endl;
            numOptions = showOptions(pokedex.getCurrentPage(), pokedex.getTotalPages());
            cin >> option;
        }

        switch (option){
            case 0: pokedex.saveToFile(); return 0;
            case 1: handleAddPokemon(pokedex); break;
            case 2: handleShowPokemon(pokedex); break;
            default: handlePageOrPokedexChange(pokedex, option); break;
        }
    }
    return 0;
}