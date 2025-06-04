#ifndef POKEMON_HPP
#define POKEMON_HPP

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

class Pokemon {
    private:
        string name;
        size_t xp;
        int pokedexID;

        void readData();
    public:
        Pokemon(string name);
        Pokemon(string name, size_t xp);
        Pokemon(const Pokemon& other);

        void setName(const string& newName);
        void setXP(size_t newXP);
        void setPokedexID(size_t id);
        
        string getName() const;
        size_t getXP() const;
        int getPokedexID() const;

        void gainXP(size_t amount);

        void serialize(ofstream& filename) const;
        void deserialize(ifstream& filename);

        friend ostream& operator<<(ostream& os, const Pokemon& pokemon);
        bool operator<(const Pokemon& other) const;
        bool operator==(const Pokemon& other) const;
};

class PokeHash {
    public:
        size_t operator()(const Pokemon& poke) const {
            return hash<string>()(poke.getName());
        }
};


#endif