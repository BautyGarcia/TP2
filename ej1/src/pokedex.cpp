#define STB_IMAGE_IMPLEMENTATION
#include "../include/pokedex.hpp"

const int GREEN_R = 0, GREEN_G = 255, GREEN_B = 0;

//esto es el ancho maximo para que se printee la pokedex, el minimo es 47 pero no tiene en cuenta este valor.
//si se pone un numero menor a 47, se acomoda la pokedex a 47 de ancho, sin embargo no se va a ver bien.
//si se ve cortada la imagen en la pokedex, proba bajando este valor para acomodar al tamaño de tu terminal.
//IMPORTANTE: numeros ideales para que este todo alineado: 51, 52, 57, 58, 63, 64, 69, 70, 75, 76, 81, 82, etc.
const int MAX_TERM_WIDTH = 75;

Pokedex::Pokedex(string name):
    saveName(name), pokes()
{}

//PokemonInfo se crea automaticamente segun el pokemon que se haya pasado
void Pokedex::addPokemon(const Pokemon& pokemon) {
    if (pokes.find(pokemon) == pokes.end()) {
        pokes[pokemon] = PokemonInfo(pokemon.getPokedexID());
        return;
    }
    else cout << "Pokemon already exists in the Pokedex!" << endl;
}

int Pokedex::getPokemonLevel(const pair<Pokemon, PokemonInfo>& entry) const {
    //devuelve el nivel del pokemon en base a su xp
    for (int i = 0; i < static_cast<int>(entry.second.getXPRemaining().size()); i++) {
        if (entry.second.getXPRemaining()[i] > entry.first.getXP()) {
            return i;
        }
    }
    return -1;
}

void Pokedex::show(const Pokemon poke) const {
    //itero por todos los pokemones de la pokedex
    for (const auto& entry : pokes){

        //si encuentro el pokemon, muestro su info
        if (entry.first == poke){
            vector<string> data = getDataToPrint(entry);
            string image = pokemonImagesRow({entry.first, Pokemon("Empty"), Pokemon("Empty")}, 30);

            //separo la imagen en lineas para printear la info a la derecha
            vector<string> imageLines;
            istringstream stream(image);
            string imageSingleLine;
            while (getline(stream, imageSingleLine)) {
                imageLines.push_back(imageSingleLine);
            }

            
            //agrego lineas vacias a los vectores para que tengan el mismo tamaño
            while (imageLines.size() < data.size()) {
                imageLines.push_back(string(30, ' '));
            }
            while (data.size() < imageLines.size()) {
                data.push_back(string(1, ' '));
            }

            for (size_t i = 0; i < imageLines.size(); i++) {
                //printeo la imagen y los datos del pokemon
                cout << imageLines[i] << data[i] << endl;
            }
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
            cout << "|" << string(MAX_TERM_WIDTH * 2 / 3, '_') << "||" << string(MAX_TERM_WIDTH * 2 / 3, '_') << "||" << string(MAX_TERM_WIDTH * 2 / 3, '_') << "|" << endl;
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
    for (const auto& poke : pokes) {
        poke.first.serialize(file);
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
        poke.deserialize(file);
        addPokemon(poke);
    }

    file.close();
}

//============ METODOS PESADOS PARA IMPRIMIR LA POKEDEX (PRIVADOS) ============//
string Pokedex::pokemonImagesRow(vector<Pokemon> pokemons, const int widthOfPrints) const {
    string output;
    vector<int> widths, heights, channels;

    //cargo los 3 pokemones de esta fila a la vez
    vector<unsigned char*> images = cargarImagenesPokemones(pokemons, widths, heights, channels);
    if (images.empty()) {
        cout << "Error al cargar una o más imágenes. Abortando..." << endl;
        return "";
    }

    //escalo la imagen para que se ajuste al ancho requerido (15 o 30)
    vector<float> scaleXs, scaleYs;
    for (size_t i = 0; i < 3; ++i) {
        scaleXs.push_back(static_cast<float>(widths[i]) / widthOfPrints);
        scaleYs.push_back(static_cast<float>(heights[i]) / widthOfPrints);
    }

    //busco la imagen mas corta en y
    for (int y = 0; y < widthOfPrints; y++) {

        //para cada linea, se alinean los 3 pokemones con un \n al final (se printean los 3 juntos linea a linea)
        for (int i = 0; i < 3; i++) {
            //veo cuantos empty hay para ver si tengo que centrar al pokemon
            int tabs = 0;
            if (pokemons[i].getName() == "Empty") break;
            if (pokemons[2].getName() == "Empty") tabs = 1;
            if (pokemons[1].getName() == "Empty") tabs = 4;
            if (widthOfPrints == 30) tabs = 0;

            output += "|";
            for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');

            //centro la imagen en cada bloque de 25 caracteres
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6; i++) {
                output += "  ";
            }

            //printeo una linea del pokemon correspondiente
            for (int x = 0; x < widthOfPrints; x++) {

                //mas calculos de pixeles que ayudo el chat
                int srcX = static_cast<int>(x * scaleXs[i]);
                int srcY = static_cast<int>(y * scaleYs[i]);
                int channel = channels[i];
                int idx = (srcY * widths[i] + srcX) * channel;
                int r = images[i][idx];
                int g = images[i][idx + 1];
                int b = images[i][idx + 2];

                bool isTransparent = false;
                if (channel == 4) { // Imagen con canal alpha
                    int a = images[i][idx + 3];
                    isTransparent = (a == 0);
                }
                //a las fotos les agregamos un fondo verde para sacarselo porque NO EXISTE UN PNG BUENO DE UN POKEMON
                //aparte es mucho mas facil que quede bien asi que con otro fondo
                if ((r == GREEN_R && g == GREEN_G && b == GREEN_B) || isTransparent) {
                    output += "  ";
                } else {
                    output += getPixelColor(r, g, b);
                }
            }

            //agrego el segundo margen
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6 ; i++) {
                output += "  ";
            }

            //agrego los tabs que se calcularon arriba
            for (int j = 0; j < tabs; j++) {
                output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
            }
            output += "|";
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
    cout << pokemonImagesRow(threePokemons, 15);
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);
    
        cout << string(MAX_TERM_WIDTH * 2 / 3, ' ');

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) {
            cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
        }
        cout << "|";
    }
    cout << endl;
    printNames(threePokemons);
    printXP(threePokemons);
    printAttacks(threePokemons);
}

void Pokedex::printNames(const vector<Pokemon> threePokemons) const {
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);
        
        //saco el nivel del pokemon
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        string levelStr = (level >= 0) ? " (Level: " + to_string(level) + ")" : " (Max Level)";

        //multiplico por 2 porque el ancho se mide con un ratio de 2 caracteres por pixel
        printCentered(threePokemons[i].getName() + levelStr, MAX_TERM_WIDTH * 2 / 3);

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) {
            cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
        }
        cout << "|";
    }
    cout << endl;
}

void Pokedex::printXP(const vector<Pokemon> threePokemons) const {
    //printeo la barra de experiencia de cada pokemon
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        float xpPercentage = static_cast<float>(threePokemons[i].getXP()) / pokes.at(threePokemons[i]).getXPRemaining()[level] * 100;
        string levelBarStr = (level >= 0) ? barCreator(xpPercentage) : barCreator(100.0f);
        cout << string(MAX_TERM_WIDTH / 3 - 13, ' ') << levelBarStr << string(MAX_TERM_WIDTH / 3 - 13, ' ');

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) {
            cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
        }
        cout << "|";
    }
    cout << endl;

    //printeo cuanta experiencia le falta al pokemon para evolucionar
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        if (level >= 0) {
            size_t xpNeeded = pokes.at(threePokemons[i]).getXPRemaining()[level];
            size_t xpCurrent = threePokemons[i].getXP();
            string xpStr = to_string(xpCurrent) + "/" + to_string(xpNeeded);
            printCentered(xpStr, MAX_TERM_WIDTH * 2 / 3);
        } else {
            printCentered("Max Level", MAX_TERM_WIDTH * 2 / 3);
        }

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) {
            cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
        }
        cout << "|";
    }
    cout << endl;
}

void Pokedex::printAttacks(const vector<Pokemon> threePokemons) const {
    //printeo el titulo de ataques
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);
        
        if (pokes.at(threePokemons[i]).getAttacks().size()) printCentered("Attacks:", MAX_TERM_WIDTH * 2 / 3);
        else printCentered(threePokemons[i].getName() + " has no attacks.", MAX_TERM_WIDTH * 2 / 3);
        
        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) {
            cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
        }
        cout << "|";
    }
    cout << endl;

    //printeo los primeros 3 ataques de cada pokemon
    for (size_t attack = 0; attack < 3; attack++) {
        for (int poke = 0; poke < 3; poke++) {
            if (threePokemons[poke].getName() == "Empty") break;
            int tabs = emptysAndLeftTabs(threePokemons, MAX_TERM_WIDTH);

            if (attack >= pokes.at(threePokemons[poke]).getAttacks().size()) {
                printCentered("", MAX_TERM_WIDTH * 2 / 3);
                if (poke < 2) cout << "|";
                continue;
            }
        
            auto attacks = pokes.at(threePokemons[poke]).getAttacks();
            string attackStr = "";
            if (attack < attacks.size()) {
                attackStr = attacks[attack].first + " (Power: " + to_string(attacks[attack].second) + ")";
            }
            printCentered(attackStr, MAX_TERM_WIDTH * 2 / 3);

            //agrego los tabs de la derecha
            for (int j = 0; j < tabs; j++) {
                cout << string(MAX_TERM_WIDTH / 6 + 1, ' ');
            }
            cout << "|";
        }
        cout << endl;
    }
}

//============ METODOS PESADOS PARA IMPRIMIR UN POKEMON (PRIVADOS) ============//
vector<string> Pokedex::getDataToPrint(pair<Pokemon, PokemonInfo> entry) const {
    vector<string> data;
    data.push_back("Name: " + entry.first.getName());
    data.push_back("Type: " + entry.second.getType());
    data.push_back(" ");
    data.push_back("Level: " + to_string(getPokemonLevel(entry)));
    data.push_back("XP: " + barCreator(static_cast<float>(entry.first.getXP()) / entry.second.getXPRemaining()[getPokemonLevel(entry)] * 100) + " " + to_string(entry.first.getXP()) + "/" + to_string(entry.second.getXPRemaining()[getPokemonLevel(entry)]));
    data.push_back(" ");
    if (entry.second.getAttacks().size()) data.push_back("\033[4mAttacks:\033[0m");
    else data.push_back(entry.first.getName() + " has no attacks.");
    
    for (const auto& attack : entry.second.getAttacks()) {
        data.push_back(" - " + attack.first + " (Power: " + to_string(attack.second) + ")");
    }
    data.push_back(" ");

    data.push_back("\033[4mDescription:\033[0m");
    size_t maxWidth = MAX_TERM_WIDTH - 31;
    stringstream ss(entry.second.getDescription());
    string line;
    getline(ss, line);

    bool firstLine = true;
    //voy a cortar el texto por si tengo que bajar de linea
    while (line.length() > maxWidth) {
        //busco el ultimo espacio antes de maxWidth (para no cortar palabras)
        size_t lastSpace = line.rfind(' ', maxWidth);

        //si no encuentro un espacio vuelvo al limite y por ser una palabra muy larga se corta
        if (lastSpace == string::npos || lastSpace == 0) lastSpace = maxWidth;

        string toPush;
        if (firstLine) {toPush += " "; firstLine = false;}
        toPush += toPush + line.substr(0, lastSpace);
        
        //como las descripciones tenian comas y se me rompia todo, las cambie por * asi que las vuelvo a cambiar a comas
        replace(toPush.begin(), toPush.end(), '*', ',');
        data.push_back(toPush);

        //skipeo el espacio y descarto lo que ya guarde
        if (line[lastSpace] == ' ') lastSpace++;
        line = line.substr(lastSpace);
    }
    
    //la ultima linea queda afuera del while asi que hago todo una vez mas
    if (!line.empty()) {
        string toPush = line;
        replace(toPush.begin(), toPush.end(), '*', ',');
        data.push_back(toPush);
    }

    return data;
}
