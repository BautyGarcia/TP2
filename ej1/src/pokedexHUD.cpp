#include "../include/pokedexHUD.hpp"
#include "../include/pokedex.hpp"
#include "../include/pokemon.hpp"

void clearScreen(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

vector<unsigned char*> cargarImagenesPokemones(vector<Pokemon> pokemons, vector<int>& widths, vector<int>& heights, vector<int>& chans) {
    vector<unsigned char*> images;

    for (size_t i = 0; i < 3 && i < pokemons.size(); i++) {
        if (pokemons[i].getName() == "Empty") {
            continue;
        }
        //construyo el path de la imagen del pokemon
        ostringstream pokedexIDStream;
        pokedexIDStream << setw(3) << setfill('0') << pokemons[i].getPokedexID();
        string pokePath = "assets/imgs/" + pokedexIDStream.str() + ".png";
        
        if (!filesystem::exists(pokePath)) {
            throw runtime_error("fatal error: " + pokePath + ": No such file or directory");
            return {};
        }

        //cargamos la imagen con una libreria externa
        int width, height, channels;
        unsigned char* img = stbi_load(pokePath.c_str(), &width, &height, &channels, 0);
        if (!img) {
            throw runtime_error(stbi_failure_reason());
            images.push_back(nullptr);
            continue;
        }
        
        images.push_back(img);
        widths.push_back(width);
        heights.push_back(height);
        chans.push_back(channels);
    }
    return images;
}

void printCentered(string& output, string text, int width) {
    int len = text.length();
    int leftMargin = (width - len) / 2;
    int rightMargin = width - len - leftMargin;
    stringstream os;
    os << setw(leftMargin + len) << setfill(' ') << text << setw(rightMargin) << setfill(' ') << "";
    output += os.str();
}

string barCreator(float percentage) {
    const int TOTAL = 26;
    const int BLOCK_SIZE = 2;
    string barColor = getPixelColor(143, 210, 246);
    string bar;
    
    //solo entran 13 bloques (cada uno ocupa dos caracteres)
    int filled_blocks = round((percentage / 100.0f) * 13);

    //meto la cantidad de bloques que corresponda
    for (int i = 0; i < filled_blocks; ++i) {
        bar += barColor;
    }

    //calculo cuantos caracteres sobraron
    int used_chars = filled_blocks * BLOCK_SIZE;
    if (filled_blocks == 13) return bar;

    //si la barra no esta llena, la relleno como que esta vacia
    bar += string(TOTAL - used_chars - 1, '_');
    bar += '|';
    return bar;
}

int showOptions(size_t currentPage, size_t totalPages) {
    cout << "1. Add Pokemon" << endl;
    cout << "2. Select a Pokemon" << endl;

    int num = 3;
    if (totalPages > 1 && currentPage < totalPages) {
        cout << num << ". Next Page" << endl;
        num++;
    }
    if (currentPage > 1 && totalPages > 1) {
        cout << num << ". Previous Page" << endl;
        num++;
    }
    cout << "0. Exit" << endl;
    cout << "> ";
    return num;
}

int emptysAndLeftTabs(string& output, vector<Pokemon> threePokemons, int MAX_TERM_WIDTH) {
    //veo cuantos empty hay para ver si tengo que centrar al pokemon
    int tabs = 0;
    if (threePokemons[2].getName() == "Empty") tabs = 1;
    if (threePokemons[1].getName() == "Empty") tabs = 4;

    output += "|";
    for (int j = 0; j < tabs; j++) {
        output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
    }

    return tabs;
}

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
void handlePageOrPokedexChange(Pokedex& pokedex, int option) {
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

void loadingScreen() {
    clearScreen();

    //flush hace que se printee el cout en cada sleep (cumple el rol de endl sin bajar de linea)
    while (isLoading){
        cout << "Uploading Data." << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
        clearScreen();
    }
    cout << "Data uploaded successfully!" << endl;
    this_thread::sleep_for(chrono::seconds(1));
}