#include "../include/pokemon.hpp"

Pokemon::Pokemon(string name):
    name(name), xp(0)
{}

Pokemon::Pokemon(string name, size_t xp):
    name(name), xp(xp)
{}

Pokemon::Pokemon(const Pokemon& other):
    name(other.name), xp(other.xp)
{}

void Pokemon::setName(const string& newName) {
    name = newName;
}

void Pokemon::setXP(size_t newXP) {
    xp = newXP;
}

string Pokemon::getName() const {
    return name;
}

size_t Pokemon::getXP() const {
    return xp;
}

void Pokemon::gainXP(size_t amount) {
    xp += amount;
}

void Pokemon::serialize(ofstream& file) const {
    //guardo el largo del nombre, el nombre en si y la cantidad de xp
    file.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
    size_t nameLength = name.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength);
}

void Pokemon::deserialize(ifstream& file) {
    //leo los datos del archivo
    file.read(reinterpret_cast<char*>(&xp), sizeof(xp));
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    file.read(&name[0], nameLength);
}

ostream& operator<<(ostream& os, const Pokemon& pokemon) {
    os << pokemon.name << " - XP: " << pokemon.xp;
    return os;
}

bool Pokemon::operator<(const Pokemon& other) const {
    return name < other.name;
}

bool Pokemon::operator==(const Pokemon& other) const {
    return name == other.name && xp == other.xp;
}