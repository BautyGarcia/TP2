#include "../include/utils.hpp"
#include "../include/pokedex.hpp"
#include "../include/pokemon.hpp"
#include "../include/pokedexHUD.hpp"

unordered_map<string, PokemonInfoMapped> pokemonDataBase;

void handleAddPokemon(Pokedex& pokedex) {
    string name;
    cout << "Enter the name of the Pokemon to add: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Enter a valid name.";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\033[A\33[2K";
        cin.clear();
        getline(cin, name);
    }
    if (!name.empty()) {
        name[0] = toupper(name[0]);
        for (size_t i = 1; i < name.size(); i++) {
            name[i] = tolower(name[i]);
        }
    }
    pokedex.addPokemon(Pokemon(name));
}

void handleShowPokemon(Pokedex& pokedex) {
    string name;
    cout << "Select a Pokemon from your Pokedex: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Enter a valid name.";
        this_thread::sleep_for(chrono::seconds(2));
        cout << "\033[A\33[2K";
        cin.clear();
        getline(cin, name);
    }
    if (!name.empty()) {
        name[0] = toupper(name[0]);
        for (size_t i = 1; i < name.size(); i++) {
            name[i] = tolower(name[i]);
        }
    }
    clearScreen();
    pokedex.show(Pokemon(name));
    cout << endl << "Press enter to continue...";
    string dummy;
    getline(cin, dummy);
}

//devuelve true si hay que cambiar de pokedex y false si solo hay que cambiar de pagina
void handlePage(Pokedex& pokedex, int option) {
    if (option == 3) {
        if (pokedex.getCurrentPage() < pokedex.getTotalPages()) pokedex.nextPage();
        else if (pokedex.getCurrentPage() > 1) pokedex.previousPage();
    }
    else if (option == 4 && pokedex.getCurrentPage() > 1) pokedex.previousPage();
}

int askForPokedex(vector<string> savedPokedexes) {
    clearScreen();
    cout << "Select a Pokedex to upload:" << endl;
    size_t num;
    for (num = 1; num < savedPokedexes.size() + 1; num++) {
        cout << num << ". " << savedPokedexes[num - 1] << endl;
    }
    cout << num << ". Create a new Pokedex" << endl;
    cout << ++num << ". Exit" << endl;
    cout << "> ";

    int option;
    string inputLine;
    getline(cin, inputLine);
    stringstream ss(inputLine);
    ss >> option;
    while (ss.fail() || option < 0 || option > static_cast<int>(num)) {
        cout << "Invalid input. Select a valid option." << endl;
        cin.clear();
        this_thread::sleep_for(chrono::seconds(1));

        clearScreen();
        cout << "Select a Pokedex to upload:" << endl;
        size_t num;
        for (num = 1; num < savedPokedexes.size() + 1; num++) {
            cout << num << ". " << savedPokedexes[num - 1] << endl;
        }
        cout << num << ". Create a new Pokedex" << endl;
        cout << ++num << ". Exit" << endl;
        cout << "> ";
        getline(cin, inputLine);
        ss.clear();
        ss.str(inputLine);
        ss >> option;
    }

    if (option == static_cast<int>(num)) return -1;
    if (option == static_cast<int>(num - 1)) return 0;
    return option;
}

//funcion para cargar los datos desde los csv a pokemonDataBase
void loadPokemonDataBase(){
    ifstream pokeDataFile("assets/data/pokemons.csv");
    ifstream movesDataFile("assets/data/moves.csv");
    string line;

    string name, type1, type2, movesList, description;

    //primero armo un map para guardar el poder de cada movimiento y usarlo mas adelante
    unordered_map<string, size_t> movePower;
    getline(movesDataFile, line);
    while (getline(movesDataFile, line)) {
        stringstream ss(line);
        string moveName, powerStr;

        //agarro el nombre del movimiento
        getline(ss, moveName, ',');

        //skipeo 3 columnas que no me interesan y guardo el poder del movimiento
        for (int i = 0; i < 4; i++) getline(ss, powerStr, ',');

        //guardo el poder del movimiento
        movePower[moveName] = static_cast<size_t>(stoi(powerStr));
    }

    //extraigo la info de cada pokemon
    getline(pokeDataFile, line);
    while (getline(pokeDataFile, line)) {
        stringstream ss(line);
        string value;

        //numero de pokedex
        getline(ss, value, ',');
        size_t pokedexID = stoi(value);

        //nombre del pokemon
        getline(ss, name, ',');

        //ambos tipos posibles
        getline(ss, type1, ',');
        getline(ss, type2, ',');

        //calculo cuanta xp necesita para subir de nivel en base a las stats
        size_t sumStats = 0;
        for (int i = 0; i < 6; i++) {
            getline(ss, value, ',');
            sumStats += stoi(value);
        }
        vector<size_t> xpRemaining = {0, sumStats * 10, sumStats * 30};

        //skipeo 4 columnas y saco todos los movimientos
        for (int i = 0; i < 5; i++) getline(ss, movesList, ',');

        //separo los movimientos en un vector
        vector<string> moveNames;
        stringstream movesStream(movesList);
        string move;
        while (getline(movesStream, move, ';')) {
            if (!move.empty()) moveNames.push_back(move);
        }

        //descripcion
        getline(ss, description, ',');

        //creo un PokemonInfoMapped y lo guardo en el map pokemonDataBase
        PokemonInfoMapped info;
        info.pokedexID = pokedexID;
        info.type = type1 + (type2.empty() ? "" : "/" + type2);
        info.description = description;
        info.xpRemaining = xpRemaining;

        //guardo cada movimiento y el poder que tiene
        for (const auto& move : moveNames) info.attacks.push_back({move, movePower[move]});

        pokemonDataBase[name] = info;
    }
};
