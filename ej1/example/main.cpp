#include "../include/pokedex.hpp"
#include "../include/pokedexHUD.hpp"
#include "../include/pokemon.hpp"
#include "../include/info.hpp"

atomic<bool> isLoading(true);

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
    string folder = "./bin";
    vector<string> savedPokedexes;

    //script de stack overflow para extraer los nombres de los archivos .bin de una carpeta
    for (const auto& entry : filesystem::directory_iterator(folder)) {
        if (filesystem::is_regular_file(entry) && entry.path().extension() == ".bin") {
            savedPokedexes.push_back(entry.path().filename().string());
        }
    }

    //el usuario elige una pokedex de las que estan guardadas
    int selectedPokedex = askForPokedex(savedPokedexes);
    if (selectedPokedex == -1) return 0;

    //si arriba se devolvio 0, se crea una nueva pokedex. Sino el nombre es el elegido y se carga la info
    string pokedexName;
    if (!selectedPokedex) {
        clearScreen();
        cout << "Write a name for your new Pokedex: ";
        string inputLine;
        getline(cin, inputLine);
        pokedexName = inputLine;
        while (pokedexName.empty()) {
            cout << "Enter a valid name.";
            cin.clear();
            this_thread::sleep_for(chrono::seconds(1));
            clearScreen();
            cout << "Write a name for your new Pokedex: ";
            getline(cin, inputLine);
            pokedexName = inputLine;
        }
    }
    else pokedexName = savedPokedexes[selectedPokedex - 1];

    //creo la respectiva
    Pokedex pokedex(pokedexName);
    if (selectedPokedex){
        //aviso que esta cargando la pokedex
        isLoading = true;

        auto startTime = high_resolution_clock::now();
        thread loading(loadingScreen);
        pokedex.loadFromFile(pokedexName);
        isLoading = false;
        loading.join();
        auto endTime = high_resolution_clock::now();
        auto elapsedTime = duration_cast<seconds>(endTime - startTime);
        cout << "Time: " << elapsedTime.count() << " seconds." << endl;
    }
    
    cout << "Waiting 2 seconds to start the Pokedex..." << endl;
    //this_thread::sleep_for(chrono::seconds(2));

    while (true){
        clearScreen();
        pokedex.show();
        cout << endl << endl;
        int numOptions = showOptions(pokedex.getCurrentPage(), pokedex.getTotalPages());

        string inputLine;
        int option;
        getline(cin, inputLine);
        stringstream ss(inputLine);
        ss >> option;
        while (ss.fail() || option < 0 || option > numOptions - 1) {
            cout << "Invalid input. Select a valid option." << endl;
            this_thread::sleep_for(chrono::seconds(1));
            clearScreen();
            pokedex.show();
            cout << endl << endl;
            numOptions = showOptions(pokedex.getCurrentPage(), pokedex.getTotalPages());
            getline(cin, inputLine);
            ss.clear();
            ss.str(inputLine);
            ss >> option;
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