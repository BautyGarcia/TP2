#include "../include/info.hpp"
#include "../include/utils.hpp"

PokemonInfo::PokemonInfo(string name):
    type(""), description(""), attacks(), xpRemaining(){
        type = pokemonDataBase[name].type;
        description = pokemonDataBase[name].description;
        attacks = pokemonDataBase[name].attacks;
        xpRemaining = pokemonDataBase[name].xpRemaining;
    }

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
