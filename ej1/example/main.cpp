#include "../include/pokedex.hpp"
#include "../include/pokedexHUD.hpp"
#include "../include/pokemon.hpp"
#include "../include/info.hpp"
#include "../include/utils.hpp"

atomic<bool> isLoading(true);

int main(){
    srand(static_cast<unsigned int>(time(nullptr)));
    //cargo todos los pokemones a la base de datos
    loadPokemonDataBase();

    //script de stack overflow para extraer los nombres de los archivos .bin de una carpeta
    string folder = "./bin";
    vector<string> savedPokedexes;
    for (const auto& entry : filesystem::directory_iterator(folder)) {
        if (filesystem::is_regular_file(entry) && entry.path().extension() == ".bin") {
            savedPokedexes.push_back(entry.path().filename().string());
        }
    }

    //el usuario elige una pokedex de las que estan guardadas o crear una nueva
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

        //mando la thread de la pantalla de carga a laburar
        thread loading(loadingScreen);
        pokedex.loadFromFile(pokedexName);

        //cuando termina de cargar los pokemones, aviso a la pantalla de carga que se puede volver a dormir
        isLoading = false;
        loading.join(); //este muñeco parece que no sirve mucho pero por temas de timing es mas seguro tenerlo
    }

    while (true){
        //limpio la terminal y muestro la pokedex con las acciones
        clearScreen();
        pokedex.show();
        cout << endl << endl;
        int numOptions = showOptions(pokedex.getCurrentPage(), pokedex.getTotalPages());

        //pido que elija la opcion
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

        //handeleo casos
        switch (option){
            case 0: pokedex.saveToFile(); return 0;
            case 1: handleAddPokemon(pokedex); break;
            case 2: handleShowPokemon(pokedex); break;
            default: handlePage(pokedex, option); break;
        }
    }
    return 0;
}