#ifndef INFO_HPP
#define INFO_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <fstream>
#include <thread>
#include <chrono>
#include <set>
#include <random>
#include <sstream>
#include <unordered_map>
using namespace std;

class PokemonInfo {
    private:
        string type;
        string description;
        vector<pair<string, size_t>> attacks;
        vector<size_t> xpRemaining;

        //pongo el constructor privado ya que este depende uncamente del pokemon al que se le asigne, no me interesa tener un 
        //PokemonInfo sin un Pokemon por lo que hago friend a Pokedex para que sea la unica forma de crear un PokemonInfo
        PokemonInfo(int pokedexID);
        void readData(int pokedexID);
    public:
        PokemonInfo() = default;

        void setType(const string& newType);
        void setDescription(const string& newDescription);
        void setAttacks(const vector<pair<string, size_t>>& newAttacks);
        void setXPRemaining(const vector<size_t>& newXPRemaining);

        string getType() const;
        string getDescription() const;
        vector<pair<string, size_t>> getAttacks() const;
        vector<size_t> getXPRemaining() const;

        void addAttack(string attackName, size_t damage, size_t power);

        friend class Pokedex;
};






#endif