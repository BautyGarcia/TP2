#include "../include/info.hpp"
#include "../include/utils.hpp"

PokemonInfo::PokemonInfo(size_t pokedexID):
    type(""), description(""), attacks(), xpRemaining(){
        type = pokemonDataBase[pokedexID].type;
        description = pokemonDataBase[pokedexID].description;
        attacks = pokemonDataBase[pokedexID].attacks;
        xpRemaining = pokemonDataBase[pokedexID].xpRemaining;
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