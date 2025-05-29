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
using namespace std;

class PokemonInfo {
    private:
        string type;
        string description;
        vector<pair<string, size_t>> attacks;
        vector<size_t> xpRemaining;

    public:
        PokemonInfo();
        PokemonInfo(string type, string description);
        PokemonInfo(string type, string description, vector<pair<string, size_t>> attacks, vector<size_t> xpRemaining);
        
        void setType(const string& newType);
        void setDescription(const string& newDescription);
        void setAttacks(const vector<pair<string, size_t>>& newAttacks);
        void setXPRemaining(const vector<size_t>& newXPRemaining);

        string getType() const;
        string getDescription() const;
        vector<pair<string, size_t>> getAttacks() const;
        vector<size_t> getXPRemaining() const;

        void addAttack(string attackName, size_t damage, size_t power);

        void serialize(ofstream& filename) const;
        void deserialize(ifstream& filename);
};






#endif