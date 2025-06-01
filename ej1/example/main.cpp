#include "../include/pokedex.hpp"

void clearScreen();
void clearScreen(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

int main(){
    clearScreen();
    Pokedex pokedex("PokeTest1");

    Pokemon squirtle("Squirtle", 10000);
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

    pokedex.addPokemon(squirtle, squirtleData);
    pokedex.addPokemon(bulbasaur, bulbasaurData);
    pokedex.addPokemon(charmander, charmanderData);

    cout << "======== Full pokedex: ========" << endl;
    pokedex.show();

    cout << "Press enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearScreen();

    cout << endl << "======== Showing individually ========" << endl;
    cout << "Showing Squirtle data:" << endl;
    pokedex.show(squirtle);

    cout << endl << "Showing Bulbasaur data with another level (different Bulbasaur):" << endl;
    pokedex.show(Pokemon ("Bulbasaur", 500));

    cout << endl << "Showing Pikachu data:" << endl;
    pokedex.show(Pokemon("Pikachu"));

    cout << "Press enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearScreen();

    cout << endl << "======== Saving and loading data ========" << endl;
    pokedex.saveToFile();
    cout << "Data saved to file." << endl;

    cout << "Creating a new Pokedex and loading data from file..." << endl;
    Pokedex newPokedex("PokeTest2");
    newPokedex.loadFromFile("PokeTest1");
    cout << "Data loaded from file." << endl;

    cout << endl << "New Pokedex after loading from file:" << endl;
    newPokedex.show();

    cout << "Press enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearScreen();

    cout << endl << "======== Adding more Pokemons to the new Pokedex ========" << endl;
    newPokedex.addPokemon(Pokemon("Pikachu", 100), PokemonInfo("Electrico", "Un pokemon electrico muy popular", {{"Impactrueno", 5}, {"Rayo", 7}, {"Trueno", 6}}, {0, 300, 800}));
    newPokedex.addPokemon(Pokemon("Squirtle", 200), squirtleData);
    newPokedex.addPokemon(Pokemon("Pikachu", 300), PokemonInfo("Electrico", "Un pokemon electrico muy popular", {{"Impactrueno", 5}, {"Rayo", 7}, {"Trueno", 6}}, {0, 300, 800}));
    newPokedex.addPokemon(Pokemon("Charmander", 400), charmanderData);
    cout << "New Pokedex after adding more Pokemons:" << endl;
    newPokedex.show();
    return 0;
}