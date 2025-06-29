#ifndef POKEDEX_HPP
#define POKEDEX_HPP

#include "libs.hpp"
#include "pokemon.hpp" //necesito el PokeHash

class Pokedex {
    private:
        size_t currentPage = 1;
        string saveName;
        mutex pokedexMapMutex;
        unordered_map<Pokemon, PokemonInfo, PokeHash> pokes;

        //aca hay mucha cosa que se encarga de imprimir la interfaz, todo lo maneja show(...) asi que no me interesa hacerlo publico
        int getPokemonLevel(const pair<Pokemon, PokemonInfo>& entry) const;
        string createPokeImages(vector<Pokemon> pokemons, int withOfPrint) const;
        void printThreePokes(string& output, vector<Pokemon> pokemons) const;
        void printNames(string& output, const vector<Pokemon> pokemons) const;
        void printXP(string& output, const vector<Pokemon> pokemons) const;
        void printAttacks(string& output, const vector<Pokemon> pokemons) const;

        vector<string> getDataToPrint(pair<Pokemon, PokemonInfo> entry) const;
    public:
        Pokedex(string name);

        string getSaveName() const { return saveName; }

        size_t getTotalPages() const;
        size_t getCurrentPage() const;
        void nextPage() { if (currentPage < getTotalPages()) currentPage++; }
        void previousPage() { if (currentPage > 1) currentPage--; }

        void addPokemon(const Pokemon& pokemon);
        void show() const;
        void show(const Pokemon poke, bool printData) const;

        void saveToFile();
        void loadFromFile(string filename);
};

void blockOfPokesToLoad(Pokedex* pokedex, const vector<Pokemon>& pokesDeserialized, size_t startOfBlock, size_t endOfBlock);
#endif