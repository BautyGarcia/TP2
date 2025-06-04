#include "../include/pokedexHUD.hpp"

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

void printCentered(string text, int width) {
    int len = text.length();
    int leftMargin = (width - len) / 2;
    int rightMargin = width - len - leftMargin;
    cout << setw(leftMargin + len) << setfill(' ') << text << setw(rightMargin) << setfill(' ') << "";
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

void showOptions(size_t currentPage, size_t totalPages) {
    cout << "1. Select a Pokemon" << endl;
    cout << "2. Add Pokemon" << endl;
    cout << "3. Remove Pokemon" << endl;

    int num = 4;
    if (totalPages > 1 && currentPage < totalPages) {
        cout << num << ". Next Page" << endl;
        num++;
    }
    if (currentPage > 1 && totalPages > 1) {
        cout << num << ". Previous Page" << endl;
        num++;
    }
    cout << num << ". Select a new Pokedex" << endl;
    cout << num << ". Exit" << endl;
}

int emptysAndLeftTabs(vector<Pokemon> threePokemons, int MAX_TERM_WIDTH) {
    //veo cuantos empty hay para ver si tengo que centrar al pokemon
    int tabs = 0;
    if (threePokemons[2].getName() == "Empty") tabs = 1;
    if (threePokemons[1].getName() == "Empty") tabs = 4;

    cout << "|";
    for (int j = 0; j < tabs; j++) {
        cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
    }

    return tabs;
}