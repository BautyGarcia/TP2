#ifndef INFO_HPP
#define INFO_HPP

#include "libs.hpp"

class PokemonInfo {
    private:
        string type;
        string description;
        vector<pair<string, int>> attacks;
        vector<size_t> xpRemaining;

        //pongo el constructor privado ya que este depende uncamente del pokemon al que se le asigne, no me interesa tener un 
        //PokemonInfo sin un Pokemon por lo que hago friend a Pokedex para que sea la unica forma de crear un PokemonInfo
        PokemonInfo(size_t pokedexID);
    public:
        PokemonInfo() = default;

        void setType(const string& newType);
        void setDescription(const string& newDescription);
        void setAttacks(const vector<pair<string, int>>& newAttacks);
        void setXPRemaining(const vector<size_t>& newXPRemaining);

        string getType() const;
        string getDescription() const;
        vector<pair<string, int>> getAttacks() const;
        vector<size_t> getXPRemaining() const;
        
        friend class Pokedex;
};






#endif