#include "../include/pokedexHUD.hpp"

vector<unsigned char*> cargarImagenesPokemones(vector<Pokemon> pokemons, vector<int>& widths, vector<int>& heights, vector<int>& chans) {
    vector<unsigned char*> images;

    for (size_t i = 0; i < 3 && i < pokemons.size(); i++) {
        string pokePath = "assets/" + pokemons[i].getName() + ".png";
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

    // Si faltan caracteres, rellená hasta el anteúltimo con _
    bar += string(TOTAL - used_chars - 1, '_');

    // Agregá el caracter final '|'
    bar += '|';

    return bar;
}
