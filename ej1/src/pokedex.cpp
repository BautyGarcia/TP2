#define STB_IMAGE_IMPLEMENTATION
#include "../include/pokedex.hpp"
#include "../include/pokedexHUD.hpp"
#include "../include/pokemon.hpp"
#include "../include/info.hpp"

//esto es el ancho maximo para que se printee la pokedex, el minimo es 47 pero no tiene en cuenta este valor.
//si se pone un numero menor a 47, se acomoda la pokedex a 47 de ancho, sin embargo no se va a ver bien.
//si se ve cortada la imagen en la pokedex, proba bajando este valor para acomodar al tamaño de tu terminal.
//IMPORTANTE: numeros ideales para que este todo alineado: 51, 52, 57, 58, 63, 64, 69, 70, 75, 76, 81, 82, etc.
const int MAX_TERM_WIDTH = 75;

//esto define cuantos pokemones como maximo pueden haber en una pagina (se recomienda un multiplo de 3)
//se puede poner cualquier valor pero si es muy grande puede llegar a cortarse por no entrar en la terminal
const int POKES_PER_PAGE = 15;

//con el ternario ese no me importa si al asignar el nombre a la pokedex el ususario escribio .bin o no
Pokedex::Pokedex(string name):
    saveName(name.ends_with(".bin") ? name : name + ".bin"), pokes()
{}

//cada pagina tiene un maximo de pokemones que se pueden mostrar, se define arriba. Esto calcula cuantas paginas hay en total
size_t Pokedex::getTotalPages() const {
    return pokes.size() / POKES_PER_PAGE + (pokes.size() % POKES_PER_PAGE != 0 ? 1 : 0);
}

size_t Pokedex::getCurrentPage() const {
    return currentPage;
}

//PokemonInfo se crea automaticamente segun el pokemon que se haya pasado, por esto puse el constructor privado
void Pokedex::addPokemon(const Pokemon& pokemon) {
    //si el pokemon no existe en la base de datos de base tenia -1 como id, por lo que nunca se lo cambiaron
    if (pokemon.getPokedexID() == -1) {
        cout << "Not even the bests pokemon masters of all time know this Pokemon." << endl;
        this_thread::sleep_for(chrono::seconds(3));
        return;
    }

    //creo el pokemon info de antemano para que no quede trabado por el lock
    PokemonInfo info(pokemon.getPokedexID());
    {
        //este lock hace que se agreguen pokemones de a uno. Lo pesado que me ahorro con el multitreading en realidad es la
        //creacion del PokemonInfo, como pueden haber hasta 800 pokemones mas o menos, hace diferencia
        lock_guard<mutex> lock(pokedexMapMutex);
        if (pokes.find(pokemon) == pokes.end()) {
            pokes[pokemon] = info;
            return;
        }
    }
    cout << "Pokemon already exists in the Pokedex!" << endl;
    this_thread::sleep_for(chrono::seconds(3));   
}

int Pokedex::getPokemonLevel(const pair<Pokemon, PokemonInfo>& entry) const {
    //devuelve el nivel del pokemon en base a su xp
    for (int i = 0; i < static_cast<int>(entry.second.getXPRemaining().size()); i++) {
        if (entry.second.getXPRemaining()[i] > entry.first.getXP()) return i;
    }
    return -1; //devuelve -1 si esta a nivel max (para verlo de afuera)
}

void Pokedex::show(const Pokemon poke) const {
    //itero por todos los pokemones de la pokedex
    for (const auto& entry : pokes){

        //si encuentro el pokemon, muestro su info
        if (entry.first == poke){
            vector<string> data = getDataToPrint(entry);
            string image = createPokeImages({entry.first, Pokemon("Empty"), Pokemon("Empty")}, 30);

            //separo la imagen en lineas para printear la info a la derecha
            vector<string> imageLines;
            istringstream stream(image);
            string imageSingleLine;
            while (getline(stream, imageSingleLine)) imageLines.push_back(imageSingleLine);
            
            //agrego lineas vacias a los vectores para que tengan el mismo tamaño (evito seg faults)
            while (imageLines.size() < data.size()) imageLines.push_back(string(1, ' '));
            while (data.size() < imageLines.size()) data.push_back(string(1, ' '));

            //printeo la imagen y los datos del pokemon
            for (size_t i = 0; i < imageLines.size(); i++) cout << imageLines[i] << data[i] << endl;
            return;
        }
    }
    cout << "Unknown Pokemon!" << endl;
}

void Pokedex::show() const {
    //agrego el titulo de la pokedex y un techito para que se vea bien
    string output = string(MAX_TERM_WIDTH + 2 - (saveName.length()) / 2 - saveName.length() % 2, '=') 
        += " " + saveName + " " 
        += string(MAX_TERM_WIDTH + 2 - (saveName.length()) / 2 , '=') += "\n";
    
    if (pokes.empty()) {
        cout << output + "This pokedex is empty :(" << endl;
        return;
    }

    output += " " + string(MAX_TERM_WIDTH * 2 + 4, '_') + " " + "\n";
    
    //voy a guardar los pokemones de a 3 que son los que entran por fila
    vector<Pokemon> threePokemons;

    //veo en que pagina me habia quedado
    int firstPoke = (currentPage - 1) * POKES_PER_PAGE;
    int currentPoke = 0;

    for (const auto& entry : pokes) {
        bool isInPage = firstPoke <= currentPoke && currentPoke < firstPoke + POKES_PER_PAGE;
        if (isInPage) threePokemons.push_back(entry.first);

        //solo entran 3 pokemones por fila (si se amontonan creo que pueden entrar 4 pero al pedo)
        if (threePokemons.size() == 3) {
            printThreePokes(output, threePokemons);

            //reseteo el vector y agrego una linea para dividir las filas
            threePokemons.clear();
            output += "|" + string(MAX_TERM_WIDTH * 2 / 3, '_') + "||" + string(MAX_TERM_WIDTH * 2 / 3, '_') + "||" + string(MAX_TERM_WIDTH * 2 / 3, '_') + "|" + "\n";
        }
        currentPoke++;
    }

    //por si quedan pokemones sin mostrar (la ultima fila pueden quedar 1 o 2 pokemones colgados)
    if (!threePokemons.empty()) {
        //completo con pokemones vacios
        int numOfEmptys = 0;
        while (threePokemons.size() < 3) {
            threePokemons.push_back(Pokemon("Empty"));
            numOfEmptys++;
        }

        //la misma funcion esta preparada para recibir uno o dos pokemones y centrar automaticamente
        printThreePokes(output, threePokemons);
        if (numOfEmptys == 1) output += "|" + string(MAX_TERM_WIDTH + 1, '_') + "||" + string(MAX_TERM_WIDTH + 1, '_') + "|" + "\n";
        if (numOfEmptys == 2) output += "|" + string(MAX_TERM_WIDTH * 2 + 4, '_') + "|" + "\n";
    }
    cout << output;
}

void Pokedex::saveToFile() {
    //abro el archivo en modo binario
    //se guarda en ej1/build/bin/
    ofstream file("bin/" + saveName, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    //guardo la cantidad de pokemones que hay
    size_t count = pokes.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    //serializo cada pokemon (no guardo la info ya que esta se genera automaticamente al cargar el pokemon)
    for (const auto& poke : pokes) poke.first.serialize(file);

    file.close();
}

//cargo los pokemones de un archivo binario (como para cargar la pokedex All Unlocked tardaba 20 segundos, use threads)
void Pokedex::loadFromFile(string filename) {
    ifstream file("bin/" + filename, ios::binary);
    if (!file.is_open()) {
        cout << "Pokedex record doesnt exists." << endl;
        return;
    }

    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    vector<Pokemon> pokesDeserialized;
    pokesDeserialized.reserve(count);

    //deserializo todos los pokemones y los guardo en un vector
    for (size_t i = 0; i < count; i++) {
        Pokemon poke("");
        poke.deserialize(file);
        pokesDeserialized.push_back(poke);
    }
    file.close();

    //veo cuantas threads hay disponibles
    size_t numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    if (numThreads > count) numThreads = count;

    //calculo cuantos y que tan grandes son los bloques en los cuales voy a dividir los threads
    size_t blockSize = count / numThreads;

    //me guardo si hay un resto en la division de arriba para que no se pierdan pokemones
    size_t pokesLeft = count % numThreads;

    vector<thread> threads;
    size_t current = 0;

    for (size_t t = 0; t < numThreads; t++) {
        //defino el inicio y fin del bloque. Arranca por el ultimo que me quede y termina en el tamaño del bloque
        //aca voy metiendo los que dije antes que iban sobrando repartidos uno para cada thread
        size_t start = current;
        size_t end = start + blockSize + (t < pokesLeft ? 1 : 0);

        threads.emplace_back(blockOfPokesToLoad, this, pokesDeserialized, start, end);
        current = end;
    }

    for (auto& th : threads) th.join();
}

//funcion que se encarga de tomar un bloque de pokemones y cargarlos en la pokedex
//esto es para asignarle a cada thread un bloque de pokemones y laburen cada una con su parte designada
void blockOfPokesToLoad(Pokedex* pokedex, const vector<Pokemon>& pokesDeserialized, size_t startOfBlock, size_t endOfBlock){
    for (size_t i = startOfBlock; i < endOfBlock; i++) pokedex->addPokemon(pokesDeserialized[i]);
}


//============ METODOS PESADOS PARA IMPRIMIR LA POKEDEX (PRIVADOS) ============//
string Pokedex::createPokeImages(vector<Pokemon> pokemons, const int widthOfPrints) const {
    string output;
    vector<int> widths, heights, channels;

    //cargo los 3 pokemones de esta fila a la vez con ayuda de la libreria externa stb_image.h
    vector<unsigned char*> images = cargarImagenesPokemones(pokemons, widths, heights, channels);
    if (images.empty()) {
        cout << "Error al cargar una o más imágenes. Abortando..." << endl;
        return "";
    }

    //escalo la imagen para que se ajuste al ancho requerido (15 o 30)
    vector<float> scaleXs, scaleYs;
    for (size_t i = 0; i < 3; i++) {
        scaleXs.push_back(static_cast<float>(widths[i]) / widthOfPrints);
        scaleYs.push_back(static_cast<float>(heights[i]) / widthOfPrints);
    }

    //las imagenes son cuadradas asi que itero el alto usando el ancho
    for (int y = 0; y < widthOfPrints; y++) {

        //para cada linea, se alinean los 3 pokemones con un \n al final (se printean los 3 juntos linea a linea)
        for (int i = 0; i < 3; i++) {
            //aca veo cuantos emptys pase para centrar la ultima fila de la pagina
            int tabs = 0;
            if (pokemons[i].getName() == "Empty") break;
            if (pokemons[2].getName() == "Empty") tabs = 1;
            if (pokemons[1].getName() == "Empty") tabs = 4;
            if (widthOfPrints == 30) tabs = 0; //por esta linea de aca no uso la funcion emptysAndLeftTabs

            output += "|";
            for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');

            //centro la imagen en cada bloque
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6; i++) output += "  ";

            //printeo una linea del pokemon correspondiente
            for (int x = 0; x < widthOfPrints; x++) {

                //calculo el pixel que me interesa con la escala que sacamos antes
                int srcX = static_cast<int>(x * scaleXs[i]);
                int srcY = static_cast<int>(y * scaleYs[i]);
                int channel = channels[i];
                int idx = (srcY * widths[i] + srcX) * channel;
                int r = images[i][idx];
                int g = images[i][idx + 1];
                int b = images[i][idx + 2];

                bool isTransparent = false;
                bool isVeryDark = (r + g + b) < 60;

                //aca me fijo si el pixel tiene canal alpha para ver si es transparente
                if (channel == 4) {
                    int a = images[i][idx + 3];
                    isTransparent = (a == 0);
                }

                //si el pixel es transparente lo dejo vacio
                if (isTransparent) output += "  ";
                else if (isVeryDark) {

                    //si el pixel sale muy oscuro hago un promedio con los de los costados para suavizar (quedaba feo sino)
                    int rSum = 0, gSum = 0, bSum = 0, n = 0;
                
                    //pixel izquierdo (me fijo que no me pase del borde y sumo RGB)
                    if (x > 0) {
                        int lidx = (srcY * widths[i] + static_cast<int>((x-1)*scaleXs[i])) * channel;
                        rSum += images[i][lidx];
                        gSum += images[i][lidx + 1];
                        bSum += images[i][lidx + 2];
                        n++;
                    }

                    //pixel derecho (me fijo que no me pase del borde y sumo RGB)
                    if (x < widthOfPrints-1) {
                        int ridx = (srcY * widths[i] + static_cast<int>((x+1)*scaleXs[i])) * channel;
                        rSum += images[i][ridx];
                        gSum += images[i][ridx + 1];
                        bSum += images[i][ridx + 2];
                        n++;
                    }

                    //saco el promedio, si los dos pixeles vecinos estan vacios va a quedar negro 
                    //en la cola de charmander pasa esto por ejemplo
                    if (n > 0) {
                        r = rSum / n;
                        g = gSum / n;
                        b = bSum / n;
                    }
                    output += getPixelColor(r, g, b);
                }
                else output += getPixelColor(r, g, b); //si no es transparente ni oscuro agrego normalmente
            }

            //agrego el segundo margen
            for (int i = 0; i < (MAX_TERM_WIDTH - 45)/6 ; i++) output += "  ";

            //agrego los tabs que se calcularon arriba pero del lado derecho
            for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
            output += "|";
        }
        //bajo de linea
        output += "\n";
    }

    //libero la memoria de las imágenes
    for (auto img : images) stbi_image_free(img);
    return output;
}

void Pokedex::printThreePokes(string& output, vector<Pokemon> threePokemons) const {
    //agrego las imagenes
    output += createPokeImages(threePokemons, 15);

    //agrego una separacion entre las imagenes y los datos
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);
    
        output += string(MAX_TERM_WIDTH * 2 / 3, ' ');

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
        output += "|";
    }
    output += "\n";
    
    //agrego toda la info a output
    printNames(output, threePokemons);
    printXP(output, threePokemons);
    printAttacks(output, threePokemons);
}

void Pokedex::printNames(string& output, const vector<Pokemon> threePokemons) const {
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);
        
        //saco el nivel del pokemon
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        string levelStr = (level >= 0) ? " (Level: " + to_string(level) + ")" : " (Max Level)";

        //multiplico por 2 porque el ancho de la terminal se mide con un ratio de 2 caracteres por pixel
        printCentered(output, threePokemons[i].getName() + levelStr, MAX_TERM_WIDTH * 2 / 3);

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
        output += "|";
    }
    output += "\n";
}

void Pokedex::printXP(string& output, const vector<Pokemon> threePokemons) const {
    //printeo la barra de experiencia de cada pokemon
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        float xpPercentage = static_cast<float>(threePokemons[i].getXP()) / pokes.at(threePokemons[i]).getXPRemaining()[level] * 100;
        string levelBarStr = (level >= 0) ? barCreator(xpPercentage) : barCreator(100.0f);
        output += string(MAX_TERM_WIDTH / 3 - 13, ' ') + levelBarStr + string(MAX_TERM_WIDTH / 3 - 13, ' ');

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
        output += "|";
    }
    output += "\n";

    //printeo cuanta experiencia le falta al pokemon para evolucionar
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);
        
        int level = getPokemonLevel({threePokemons[i], pokes.at(threePokemons[i])});
        if (level >= 0) {
            size_t xpNeeded = pokes.at(threePokemons[i]).getXPRemaining()[level];
            size_t xpCurrent = threePokemons[i].getXP();
            string xpStr = to_string(xpCurrent) + "/" + to_string(xpNeeded);
            printCentered(output, xpStr, MAX_TERM_WIDTH * 2 / 3);
        } 
        else printCentered(output, "Max Level", MAX_TERM_WIDTH * 2 / 3);

        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
        output += "|";
    }
    output += "\n";
}

void Pokedex::printAttacks(string& output, const vector<Pokemon> threePokemons) const {
    //printeo el titulo de ataques
    for (int i = 0; i < 3; i++) {
        if (threePokemons[i].getName() == "Empty") break;
        int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);
        
        if (pokes.at(threePokemons[i]).getAttacks().size()) printCentered(output, "Attacks:", MAX_TERM_WIDTH * 2 / 3);
        else printCentered(output, threePokemons[i].getName() + " has no attacks.", MAX_TERM_WIDTH * 2 / 3);
        
        //agrego los tabs de la derecha
        for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
        output += "|";
    }
    output += "\n";

    //printeo los primeros 3 ataques de cada pokemon
    for (size_t attack = 0; attack < 3; attack++) {
        for (int poke = 0; poke < 3; poke++) {
            if (threePokemons[poke].getName() == "Empty") break;
            int tabs = emptysAndLeftTabs(output, threePokemons, MAX_TERM_WIDTH);

            if (attack >= pokes.at(threePokemons[poke]).getAttacks().size()) {
                printCentered(output, "", MAX_TERM_WIDTH * 2 / 3);
                if (poke < 2) output += "|";
                continue;
            }
        
            auto attacks = pokes.at(threePokemons[poke]).getAttacks();
            string attackStr = "";
            if (attack < attacks.size()) attackStr = attacks[attack].first + " (Power: " + to_string(attacks[attack].second) + ")";
            printCentered(output, attackStr, MAX_TERM_WIDTH * 2 / 3);

            //agrego los tabs de la derecha
            for (int j = 0; j < tabs; j++) output += string(MAX_TERM_WIDTH / 6 + 1, ' ');
            output += "|";
        }
        output += "\n";
    }
}

//============ METODOS PESADOS PARA IMPRIMIR UN POKEMON (PRIVADOS) ============//
vector<string> Pokedex::getDataToPrint(pair<Pokemon, PokemonInfo> entry) const {
    vector<string> data;

    //le agrego a data de a poco toda la info que me interesa mostrar del pokemon
    data.push_back("Pokedex ID: " + to_string(entry.first.getPokedexID()));
    data.push_back("Name: " + entry.first.getName());
    data.push_back("Type: " + entry.second.getType());
    data.push_back(" ");

    //-1 significa que esta a maximo nivel
    if (getPokemonLevel(entry) == -1){
        data.push_back("Max Level Reached!");
        data.push_back("XP: " + barCreator(100.0f));
    }
    else{
        data.push_back("Level: " + to_string(getPokemonLevel(entry)));
        data.push_back("XP: " + barCreator(static_cast<float>(entry.first.getXP()) / entry.second.getXPRemaining()[getPokemonLevel(entry)] * 100) + " " + to_string(entry.first.getXP()) + "/" + to_string(entry.second.getXPRemaining()[getPokemonLevel(entry)]));
    }
    data.push_back(" ");
    if (entry.second.getAttacks().size()) data.push_back("\033[4mAttacks:\033[0m");
    else data.push_back(entry.first.getName() + " has no attacks.");
    
    for (const auto& attack : entry.second.getAttacks()) data.push_back(" - " + attack.first + " (Power: " + to_string(attack.second) + ")");
    data.push_back(" ");

    data.push_back("\033[4mDescription:\033[0m");

    //a partir de aca agrego la descripcion, como es larga voy a seccionarla para que se printee en renglones
    size_t maxWidth = MAX_TERM_WIDTH - 31;
    stringstream ss(entry.second.getDescription());
    string line;
    getline(ss, line);

    bool firstLine = true;
    //corto el texto por si tengo que bajar de linea
    while (line.length() > maxWidth) {
        //busco el ultimo espacio antes de maxWidth (para no cortar palabras)
        size_t lastSpace = line.rfind(' ', maxWidth);

        //si no encuentro un espacio vuelvo al limite y por ser una palabra muy larga se corta (caso muy borde creo que nunca se da)
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