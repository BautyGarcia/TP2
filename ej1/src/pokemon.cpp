#include "../include/pokemon.hpp"

Pokemon::Pokemon(string n):
    name(n), xp(0), pokedexID(-1)
{readData();}

Pokemon::Pokemon(string n, size_t x):
    name(n), xp(x), pokedexID(-1)
{readData();}

Pokemon::Pokemon(const Pokemon& other):
    name(other.name), xp(other.xp), pokedexID(other.pokedexID)
{}

void Pokemon::setName(const string& newName) {
    name = newName;
}

void Pokemon::setXP(size_t newXP) {
    xp = newXP;
}

void Pokemon::setPokedexID(size_t id) {
    pokedexID = id;
}

string Pokemon::getName() const {
    return name;
}

size_t Pokemon::getXP() const {
    return xp;
}

int Pokemon::getPokedexID() const {
    return pokedexID;
}

void Pokemon::gainXP(size_t amount) {
    xp += amount;
}

void Pokemon::serialize(ofstream& file) const {
    //guardo el largo del nombre, el nombre en si y la cantidad de xp
    file.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), static_cast<streamsize>(nameLength));

    //guardo el id del pokedex
    file.write(reinterpret_cast<const char*>(&pokedexID), sizeof(pokedexID));
}

void Pokemon::deserialize(ifstream& file) {
    //leo los datos del archivo
    file.read(reinterpret_cast<char*>(&xp), sizeof(xp));
    
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    file.read(&name[0], static_cast<streamsize>(nameLength));

    file.read(reinterpret_cast<char*>(&pokedexID), sizeof(pokedexID));
}

ostream& operator<<(ostream& os, const Pokemon& pokemon) {
    os << pokemon.name << " - XP: " << pokemon.xp;
    return os;
}

bool Pokemon::operator<(const Pokemon& other) const {
    return name < other.name;
}

bool Pokemon::operator==(const Pokemon& other) const {
    return name == other.name;
}

//============ READ DATA (PRIVADO) ============//
void Pokemon::readData() {
    ifstream file("assets/data/pokemons.csv");
    string line;
    if (!file.is_open()) {
        cerr << "Error opening pokemons data file." << endl;
        return;
    }

    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        
        //agarro el numero de pokedex
        getline(ss, value, ',');
        int number = stoi(value);

        //agarro el nombre del pokemon
        getline(ss, value, ',');
        string nameInFile = value;

        if (nameInFile == name) {
            pokedexID = number;
            break;
        }
    }
    file.close();
}
