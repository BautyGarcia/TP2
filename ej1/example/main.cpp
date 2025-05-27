#include "../include/pokedex.hpp"

int main(){
    cout << "Creating Pokedex..." << endl;
    Pokedex pokedex;

    cout << "Creating Pokemons..." << endl;
    Pokemon squirtle("Squirtle", 100);
    Pokemon bulbasaur("Bulbasaur", 270);
    Pokemon charmander("Charmander", 633);

    vector<pair<string, size_t>> squirtleAttacks = {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}};
    vector<pair<string, size_t>> bulbasaurAttacks = {{"Latigazo", 4}, {"Hoja Afilada", 6}, {"Rayo Solar", 5}};
    vector<pair<string, size_t>> charmanderAttacks = {{"Ascuas", 4}, {"Lanzallamas", 6}, {"Giro Fuego", 5}};
    
    vector<size_t> squirtleXP = {0, 400, 1000};
    vector<size_t> bulbasaurXP = {0, 300, 1100};
    vector<size_t> charmanderXP = {0, 250, 1300};

    PokemonInfo squirtleData("Agua", "Una tortuga pequeña que lanza chorros de agua", squirtleAttacks, squirtleXP);
    PokemonInfo bulbasaurData("Planta", "Tiene una semilla en su lomo que crece con el tiempo", bulbasaurAttacks, bulbasaurXP);
    PokemonInfo charmanderData("Fuego", "Una lagartija con una llama en su cola", charmanderAttacks, charmanderXP);

    cout << "Adding Pokemons to Pokedex..." << endl;
    pokedex.addPokemon(squirtle, squirtleData);
    pokedex.addPokemon(bulbasaur, bulbasaurData);
    pokedex.addPokemon(charmander, charmanderData);

    cout << endl << "Pokedex after adding Pokemons:" << endl;
    pokedex.displayPokemon();

    cout << endl << "Showing Squirtle data:" << endl;
    pokedex.show(squirtle);

    cout << endl << "Showing Pikachu data:" << endl;
    pokedex.show(Pokemon("Pikachu"));

    cout << endl << "Saving data..." << endl;
    pokedex.saveToFile("pokedex_data");

    cout << "Creating a new Pokedex instance and loading data from file..." << endl;
    Pokedex newPokedex;
    newPokedex.loadFromFile("pokedex_data");
    cout << endl << "New Pokedex after loading from file:" << endl;
    newPokedex.displayPokemon();
    return 0;
}