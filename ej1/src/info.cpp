#include "../include/info.hpp"

PokemonInfo::PokemonInfo():
    type(""), description(""), attacks(), xpRemaining()
{}

PokemonInfo::PokemonInfo(string t, string d):
    type(t), description(d), attacks(), xpRemaining()
{}

PokemonInfo::PokemonInfo(string t, string d, vector<pair<string, size_t>> a, vector<size_t> x):
    type(t), description(d), attacks(a), xpRemaining(x) 
{}

void PokemonInfo::setType(const string& newType) {
    type = newType;
}

void PokemonInfo::setDescription(const string& newDescription) {
    description = newDescription;
}

void PokemonInfo::setAttacks(const vector<pair<string, size_t>>& newAttacks) {
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

vector<pair<string, size_t>> PokemonInfo::getAttacks() const {
    return attacks;
}

vector<size_t> PokemonInfo::getXPRemaining() const {
    return xpRemaining;
}

void PokemonInfo::addAttack(string attackName, size_t damage, size_t xpNeeded){
    attacks.push_back(make_pair(attackName, damage));
    xpRemaining.push_back(xpNeeded);
}

void PokemonInfo::serialize(ofstream& file) const {
    //guardo el largo y el str del tipo de pokemon
    streamsize typeLength = static_cast<streamsize>(type.size());
    file.write(reinterpret_cast<const char*>(&typeLength), sizeof(typeLength));
    file.write(type.c_str(), typeLength);

    //guardo el largo y el str de la descripcion
    streamsize descLength = static_cast<streamsize>(description.size());
    file.write(reinterpret_cast<const char*>(&descLength), sizeof(descLength));
    file.write(description.c_str(), descLength);

    //guardo la cantidad de ataques (siempre 3) y cada ataque con su nombre, largo del nombre y daño
    streamsize attackCount = static_cast<streamsize>(attacks.size());
    file.write(reinterpret_cast<const char*>(&attackCount), sizeof(attackCount));
    for (const auto& attack : attacks) {
        streamsize nameLen = static_cast<streamsize>(attack.first.size());
        file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        file.write(attack.first.c_str(), nameLen);
        file.write(reinterpret_cast<const char*>(&attack.second), sizeof(attack.second));
    }

    //lo mismo con los valores de xp
    streamsize xpCount = static_cast<streamsize>(xpRemaining.size());
    file.write(reinterpret_cast<const char*>(&xpCount), sizeof(xpCount));
    for (const auto& xp : xpRemaining)
        file.write(reinterpret_cast<const char*>(&xp), sizeof(xp));
}

void PokemonInfo::deserialize(ifstream& file) {
    streamsize len;

    //extraigo el largo del tipo de pokemon, modifico el tamaño del atributo y guardo el str
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    type.resize(static_cast<long unsigned int>(len));
    file.read(&type[0], static_cast<streamsize>(len));

    //idem para la descripcion
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    description.resize(static_cast<long unsigned int>(len));
    file.read(&description[0], len);

    //extraigo la cantidad de ataques, y luego cada ataque con su nombre y daño
    streamsize count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    attacks.clear();
    for (int i = 0; i < count; i++) {
        streamsize nameLen;
        file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string name(static_cast<long unsigned int>(nameLen), ' ');
        file.read(&name[0], nameLen);
        size_t dmg;
        file.read(reinterpret_cast<char*>(&dmg), sizeof(dmg));
        attacks.emplace_back(name, dmg);
    }

    //extraigo la cantidad de xp restantes y los valores
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    xpRemaining.clear();
    for (int i = 0; i < count; i++) {
        size_t xp;
        file.read(reinterpret_cast<char*>(&xp), sizeof(xp));
        xpRemaining.push_back(xp);
    }
}