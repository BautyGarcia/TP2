#ifndef LIBS_HPP
#define LIBS_HPP

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
#include <filesystem>
#include <regex>
#include <atomic>
#include <mutex>
#include "stb_image.h"
using namespace std;
using namespace chrono;

class Pokemon;
class PokeHash;
class Pokedex;
class PokemonInfo;

//uso una flag para manejar el mensaje de loading
extern atomic<bool> isLoading;

#endif