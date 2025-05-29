#include "../include/pokedex.hpp"

Pokedex::Pokedex(string name):
    saveName(name), pokes()
{}

void Pokedex::addPokemon(const Pokemon& pokemon, const PokemonInfo& info) {
    pokes[pokemon] = info;
}

void Pokedex::removePokemon(const string& name) {
    //busco el pokemon y lo elimino si existe
    for (auto it = pokes.begin(); it != pokes.end(); ++it) {
        if (it->first == name) {
            pokes.erase(it);
            return;
        }
    }
}

void Pokedex::show() const {
    //itero por todos los pokemones de la pokedex
    for (const auto& entry : pokes) {

        //muestro la info de cada uno
        cout << entry.first << " - Type: " << entry.second.getType() << ", Description: " << entry.second.getDescription() << endl;
        cout << "Attacks: ";
        for (size_t i = 0; i < entry.second.getAttacks().size(); i++){
            cout << entry.second.getAttacks()[static_cast<size_t>(i)].first << " (Power: " << entry.second.getAttacks()[i].second << ")";
            if (i < entry.second.getAttacks().size() - 1) cout << ", ";
        }
        size_t i;
        for (i = 0; i < entry.second.getXPRemaining().size(); i++){
            if (entry.second.getXPRemaining()[i] > entry.first.getXP()){
                cout << endl << "XP for next evolution: " << entry.second.getXPRemaining()[i] - entry.first.getXP() << endl;
                break;
            }
        }
        if (i == entry.second.getXPRemaining().size()) {
            cout << "Max Level" << endl;
        }
        cout << endl;
    }
}

void Pokedex::show(const Pokemon poke) const {
    //itero por todos los pokemones de la pokedex
    for (const auto& entry : pokes){

        //si encuentro el pokemon, muestro su info
        if (entry.first == poke){
            cout << entry.first << " - Type: " << entry.second.getType() << ", Description: " << entry.second.getDescription() << endl;
            cout << "Attacks: ";
            for (size_t i = 0; i < entry.second.getAttacks().size(); i++){
                cout << entry.second.getAttacks()[i].first << " (Power: " << entry.second.getAttacks()[i].second << ")";
                if (i < entry.second.getAttacks().size() - 1) cout << ", ";
            }
            for (size_t i = 0; i < entry.second.getXPRemaining().size(); i++){
                if (entry.second.getXPRemaining()[i] > entry.first.getXP()){
                    cout << endl << "XP for next evolution: " << entry.second.getXPRemaining()[i] - entry.first.getXP() << endl;
                    return;
                }
            }
            cout << "Max Level" << endl;
            return;
        }
    }
    cout << "Unknown Pokemon!" << endl;
}

void Pokedex::saveToFile() {
    //abro el archivo en modo binario
    //se guarda en ej1/build/bin/
    ofstream file("bin/" + saveName + ".bin", ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    //guardo la cantidad de pokemones que hay
    size_t count = pokes.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    //serializo cada pokemon y su info
    for (const auto& [poke, info] : pokes) {
        poke.serialize(file);
        info.serialize(file);
    }

    file.close();
}

void Pokedex::loadFromFile(const string& filename) {
    //busco el archivo para cargar
    ifstream file("bin/" + filename + ".bin", ios::binary);
    if (!file.is_open()) {
        cout << "Pokedex record doesnt exists." << endl;
        return;
    }

    //veo cuantos pokemones se habian guardado
    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    //agrego los pokemones extraidos a la pokedex
    for (size_t i = 0; i < count; i++) {
        Pokemon poke("");
        PokemonInfo info;
        poke.deserialize(file);
        info.deserialize(file);
        addPokemon(poke, info);
    }

    file.close();
}