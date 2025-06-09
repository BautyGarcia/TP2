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
        //si paso un empty no cargo la imagen. Las otras funciones estan pensadas para poder recibir menos de 3 imagenes
        if (pokemons[i].getName() == "Empty") continue;

        //construyo el path de la imagen del pokemon (los nombres son 004.png, 025.png, 347.png, etc)
        ostringstream pokedexIDStream;
        pokedexIDStream << setw(3) << setfill('0') << pokemons[i].getPokedexID();
        string pokePath = "assets/imgs/" + pokedexIDStream.str() + ".png";
        
        //verifico que exista el archivo pero en este punto ya esta mas que chequeado que llego un ID valido
        if (!filesystem::exists(pokePath)) {
            throw runtime_error("fatal error: " + pokePath + ": No such file or directory");
            return {};
        }

        //cargamos la imagen con una libreria externa (stbi_load) devuelve un string con los valores RGB de cada pixel
        int width, height, channels;
        unsigned char* img = stbi_load(pokePath.c_str(), &width, &height, &channels, 0);
        if (!img) {
            throw runtime_error(stbi_failure_reason());
            images.push_back(nullptr);
            continue;
        }

        //aca arriba uso dos throws porque:
        // 1. si no existe el archivo es porque se colo un pokemon sin ID y no deberia pasar
        // 2. todas las imagenes se pudieron cargar, si alguna falla en algun momento es por algun error de stb_image.h
        //ambos casos son errores que ameritan cortar el programa para darles toda la atencion.
        //igual, nuevamente, nunca deberian fallar a no ser que se empiece a tocar el codigo o las imagenes
        
        //agrego la info
        images.push_back(img);
        widths.push_back(width);
        heights.push_back(height);
        chans.push_back(channels);
    }
    return images;
}

//son calculos aburridos para centrar un texto en un ancho determinado
//calculo los margenes y agrego al output
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
    //trate de buscar un color que se parezca al azul de los juegos de pokemon pero ni con chatGPT lo consegui, es lo mejor que pude
    string barColor = getPixelColor(143, 210, 246); 
    string bar;
    
    //solo entran 13 bloques (cada uno ocupa dos caracteres)
    int filled_blocks = round((percentage / 100.0f) * 13);

    //meto la cantidad de bloques que corresponda
    for (int i = 0; i < filled_blocks; ++i) bar += barColor;

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
    cout << "3. Wanna see a good image" << endl;
    int num = 4;
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
    for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');

    return tabs;
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