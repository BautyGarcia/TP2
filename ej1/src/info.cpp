#include "../include/info.hpp"

PokemonInfo::PokemonInfo(int pokedexID):
    type(""), description(""), attacks(), xpRemaining()
{readData(pokedexID);}

void PokemonInfo::setType(const string& newType) {
    type = newType;
}

void PokemonInfo::setDescription(const string& newDescription) {
    description = newDescription;
}

void PokemonInfo::setAttacks(const vector<pair<string, size_t> >& newAttacks) {
    attacks = newAttacks;
}

void PokemonInfo::setXPRemaining(const vector<size_t>& newXPRemaining) {
    xpRemaining = newXPRemaining;
}

string PokemonInfo::getType() const {
    return type;
}

string PokemonInfo::getDescription() const {
    return description;
}

vector<pair<string, size_t> > PokemonInfo::getAttacks() const {
    return attacks;
}

vector<size_t> PokemonInfo::getXPRemaining() const {
    return xpRemaining;
}

void PokemonInfo::addAttack(string attackName, size_t damage, size_t xpNeeded){
    attacks.push_back(make_pair(attackName, damage));
    xpRemaining.push_back(xpNeeded);
}

//============ READ DATA (PRIVADO) ============//
void PokemonInfo::readData(int pokedexId) {
    ifstream pokeDataFile("assets/data/pokemons.csv");
    ifstream movesDataFile("assets/data/moves.csv");
    string line;

    getline(pokeDataFile, line);
    string type1, type2, movesList;
    while (getline(pokeDataFile, line)) {
        stringstream ss(line);
        string value;
        
        //busco el numero de pokedex
        getline(ss, value, ',');
        int currentId = stoi(value);

        if (currentId == pokedexId) {

            //skipeo la columna del nombre
            getline(ss, value, ',');

            //agarro los dos tipos posibles
            getline(ss, type1, ',');
            getline(ss, type2, ',');

            //la xp necesaria para subir de nivel lo determinan las stats 
            //(me sobraba info y queria que esto no sea generico ni aleatorio)
            size_t sumStats = 0;
            string currentStat;
            for (int i = 0; i < 6; i++){
                getline(ss, currentStat, ',');
                sumStats += stoi(currentStat);
            }
            xpRemaining = {0, sumStats * 10, sumStats * 30};

            //skipeo 4 lineas mas y agarro la lista de movimientos
            for (int i = 0; i < 5; i++) getline(ss, movesList, ',');

            //agarro la descripcion (la guardo de una)
            getline(ss, description, ',');
            break;
        }
    }

    if (type2.empty()) type = type1;
    else type = type1 + "/" + type2;

    //separo todos los movimientos en un vector
    vector<string> moveNames;
    stringstream movesStream(movesList);
    string move;
    while (getline(movesStream, move, ';')) {
        if (!move.empty()) moveNames.push_back(move);
    }

    //armo un map para guardar el poder de cada movimiento
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

    //busco en el vector de movimientos el poder que figura en el map
    for (const auto& move : moveNames) attacks.push_back({move, movePower[move]});

    //para no saturar la terminal, limito la cantidad de ataques a 3 (como en la consigna basicamente)
    attacks.resize(min(attacks.size(), static_cast<size_t>(3)));

    pokeDataFile.close();
    movesDataFile.close();
}
