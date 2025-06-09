#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

// defino la cantidad de zonas porque nose quien dijo que tiene que ser "escalable" esto
#define NUM_ZONAS 5

using namespace std;

class DronCamp {
    private:
        mutex zonas[NUM_ZONAS];
        mutex output;
        vector<thread> drones;
    public:
        void simulate();
        void flight(int drone);
        ~DronCamp() = default;
};