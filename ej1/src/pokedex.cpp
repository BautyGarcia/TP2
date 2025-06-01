#define STB_IMAGE_IMPLEMENTATION
#include "../include/pokedex.hpp"

const int GREEN_R = 0, GREEN_G = 255, GREEN_B = 0;

//esto es el ancho maximo para que se printee la pokedex, el minimo es 47 pero no tiene en cuenta este valor.
//si se pone un numero menor a 47, se acomoda la pokedex a 47 de ancho, sin embargo no se va a ver bien.
//si se ve cortada la imagen en la pokedex, proba bajando este valor para acomodar al tamaño de tu terminal.
//IMPORTANTE: numeros ideales para que este todo alineado: 51, 52, 57, 58, 63, 64, 69, 70, 75, 76
const int MAX_TERM_WIDTH = 75;

Pokedex::Pokedex(string name):
    saveName(name), pokes()
{}

void Pokedex::addPokemon(const Pokemon& pokemon, const PokemonInfo& info) {
    pokes[pokemon] = info;
}

void Pokedex::removePokemon(const string& name) {
    //busco el pokemon y lo elimino si existe
    for (auto it = pokes.begin(); it != pokes.end(); ++it) {
        if (it->first == name) {
            pokes.erase(it);
            return;
        }
    }
}

int Pokedex::getPokemonLevel(const pair<Pokemon, PokemonInfo>& entry) const {
    //devuelve el nivel del pokemon en base a su xp
    for (size_t i = 0; i < entry.second.getXPRemaining().size(); i++) {
        if (entry.second.getXPRemaining()[i] > entry.first.getXP()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Pokedex::show(const Pokemon poke) const {
    //itero por todos los pokemones de la pokedex
    for (const auto& entry : pokes){

        //si encuentro el pokemon, muestro su info
        if (entry.first == poke){
            cout << entry.first << " - Type: " << entry.second.getType() << ", Description: " << entry.second.getDescription() << endl;
            cout << "Attacks: ";
            for (size_t i = 0; i < entry.second.getAttacks().size(); i++){
                cout << entry.second.getAttacks()[i].first << " (Power: " << entry.second.getAttacks()[i].second << ")";
                if (i < entry.second.getAttacks().size() - 1) cout << ", ";
            }
            for (size_t i = 0; i < entry.second.getXPRemaining().size(); i++){
                if (entry.second.getXPRemaining()[i] > entry.first.getXP()){
                    cout << endl << "XP for next evolution: " << entry.second.getXPRemaining()[i] - entry.first.getXP() << " (Level: " << i << ")" << endl;
                    return;
                }
            }
            cout << "Max Level" << endl;
            return;
        }
    }
    cout << "Unknown Pokemon!" << endl;
}

void Pokedex::show() const {
    vector<Pokemon> threePokemons;
    for (const auto& entry : pokes) {
        threePokemons.push_back(entry.first);

        //solo entran 3 pokemones por fila (si se amontonan creo que pueden entrar 4 pero al pedo)
        if (threePokemons.size() == 3) {
            printThreePokes(threePokemons);
            threePokemons.clear();
            cout << string(MAX_TERM_WIDTH * 2 / 3, '_') << "|" << string(MAX_TERM_WIDTH * 2 / 3, '_') << "|" << string(MAX_TERM_WIDTH * 2 / 3, '_') << endl;
        }
    }

    //por si quedan pokemones sin mostrar
    if (!threePokemons.empty()) {
        //completo con pokemones vacios
        while (threePokemons.size() < 3) {
            threePokemons.push_back(Pokemon("Empty"));
        }
        printThreePokes(threePokemons);
    }
}

void Pokedex::saveToFile() {
    //abro el archivo en modo binario
    //se guarda en ej1/build/bin/
    ofstream file("bin/" + saveName + ".bin", ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    //guardo la cantidad de pokemones que hay
    size_t count = pokes.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    //serializo cada pokemon y su info
    for (const auto& [poke, info] : pokes) {
        poke.serialize(file);
        info.serialize(file);
    }

    file.close();
}

void Pokedex::loadFromFile(const string& filename) {
    //busco el archivo para cargar
    ifstream file("bin/" + filename + ".bin", ios::binary);
    if (!file.is_open()) {
        cout << "Pokedex record doesnt exists." << endl;
        return;
    }

    //veo cuantos pokemones se habian guardado
    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    //agrego los pokemones extraidos a la pokedex
    for (size_t i = 0; i < count; i++) {
        Pokemon poke("");
        PokemonInfo info;
        poke.deserialize(file);
        info.deserialize(file);
        addPokemon(poke, info);
    }

    file.close();
}

//============ METODOS PESADOS PARA IMPRIMIR LA POKEDEX (PRIVADOS) ============//
string Pokedex::pokemonImagesRow(vector<Pokemon> pokemons) const {
    //ancho maximo para cada imagen
    const int withOfPrints = 15;
    string output;
    vector<int> widths, heights, channels;

    //cargo los 3 pokemones de esta fila a la vez
    vector<unsigned char*> images = cargarImagenesPokemones(pokemons, widths, heights, channels);
    if (images.empty()) {
        cout << "Error al cargar una o más imágenes. Abortando..." << endl;
        return "";
    }

    //calculos de dimensiones y escalas que ayudo el chat
    vector<int> termWidths, termHeights;
    vector<float> scaleXs, scaleYs;
    for (size_t i = 0; i < images.size(); ++i) {
        termWidths.push_back((widths[i] > withOfPrints) ? withOfPrints : widths[i]);
        termHeights.push_back(heights[i] * termWidths[i] / widths[i]);
        scaleXs.push_back((float)widths[i] / termWidths[i]);
        scaleYs.push_back((float)heights[i] / termHeights[i]);
    }

    //no tienen mucha diferencia de altura por lo que agarro la mas chica, se cortan poco las grandes
    int minTermHeight = *min_element(termHeights.begin(), termHeights.end());
    for (int y = 0; y < minTermHeight; y++) {

        //para cada linea, se alinean los 3 pokemones con un \n al final (se printean los 3 juntos linea a linea)
        for (int i = 0; i < 3; i++) {
            if (pokemons[i].getName() == "Empty") break;
        

            //centro la imagen en cada bloque de 25 caracteres
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6; i++) {
                output += "  ";
            }

            //printeo una linea del pokemon correspondiente
            for (int x = 0; x < termWidths[i]; x++) {

                //mas calculos de pixeles que nos ayudo el chat
                int srcX = (int)(x * scaleXs[i]);
                int srcY = (int)(y * scaleYs[i]);
                int channel = channels[i];
                int idx = (srcY * widths[i] + srcX) * channel;
                int r = images[i][idx];
                int g = images[i][idx + 1];
                int b = images[i][idx + 2];

                //a las fotos les agregamos un fondo verde para sacarselo porque NO EXISTE UN PNG BUENO DE UN POKEMON
                //aparte es mucho mas facil que quede bien asi que con otro fondo
                if (r == GREEN_R && g == GREEN_G && b == GREEN_B) {
                    output += "  ";
                } else {
                    output += getPixelColor(r, g, b);
                }
            }

            //agrego el segundo margen
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6 ; i++) {
                output += "  ";
            }
            if (i < 2) output += "|";
        }
        output += "\n";
    }

    //libero la memoria de las imágenes
    for (auto img : images) {
        stbi_image_free(img);
    }
    return output;
}

void Pokedex::printThreePokes(vector<Pokemon> threePokemons) const {
    //printeo las imagenes
    cout << pokemonImagesRow(threePokemons);
    for (int i = 0; i < 2; i++) {
        if (threePokemons[i].getName() == "Empty") break;
    
        cout << string(MAX_TERM_WIDTH * 2 / 3, ' ');
        if (i < 2) cout << "|";
    }
    cout << endl;
    printNames(threePokemons);
    printXP(threePokemons);
    printAttacks(threePokemons);
}

void Pokedex::printNames(const vector<Pokemon> threePokemons) const {
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        
        //saco el nivel del pokemon
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        string levelStr = (level >= 0) ? " (Level: " + to_string(level) + ")" : " (Max Level)";

        //multiplico por 2 porque el ancho se mide con un ratio de 2 caracteres por pixel
        printCentered(threePokemons[i].getName() + levelStr, MAX_TERM_WIDTH * 2 / 3);
        if (i < 2) cout << "|";
    }
    cout << endl;
}

void Pokedex::printXP(const vector<Pokemon> threePokemons) const {
    //printeo la barra de experiencia de cada pokemon
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        float xpPercentage = static_cast<float>(threePokemons[i].getXP()) / pokes.at(threePokemons[i]).getXPRemaining()[level] * 100;
        string levelBarStr = (level >= 0) ? barCreator(xpPercentage) : barCreator(100.0f);
        cout << string(MAX_TERM_WIDTH / 3 - 13, ' ') << levelBarStr << string(MAX_TERM_WIDTH / 3 - 13, ' ');
        if (i < 2) cout << "|";
    }
    cout << endl;

    //printeo cuanta experiencia le falta al pokemon para evolucionar
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        if (level >= 0) {
            size_t xpNeeded = pokes.at(threePokemons[i]).getXPRemaining()[level];
            size_t xpCurrent = threePokemons[i].getXP();
            string xpStr = to_string(xpCurrent) + "/" + to_string(xpNeeded);
            printCentered(xpStr, MAX_TERM_WIDTH * 2 / 3);
        } else {
            printCentered("Max Level", MAX_TERM_WIDTH * 2 / 3);
        }
        if (i < 2) cout << "|";
    }
    cout << endl;
}

void Pokedex::printAttacks(const vector<Pokemon> threePokemons) const {
    //printeo el titulo de ataques
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        
        printCentered("Attacks:", MAX_TERM_WIDTH * 2 / 3);
        if (i < 2) cout << "|";
    }
    cout << endl;

    //printeo los ataques de cada pokemon
    for (size_t attack = 0; attack < 3; attack++) {
        for (int poke = 0; poke < 3; poke++) {
            if (threePokemons[poke].getName() == "Empty") break;
        
            auto attacks = pokes.at(threePokemons[poke]).getAttacks();
            string attackStr = "";
            if (attack < attacks.size()) {
                attackStr = attacks[attack].first + " (Power: " + to_string(attacks[attack].second) + ")";
            }
            printCentered(attackStr, MAX_TERM_WIDTH * 2 / 3);
            if (poke < 2) cout << "|";
        }
        cout << endl;
    }
}