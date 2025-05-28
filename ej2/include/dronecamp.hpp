#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

class DronCamp {
    private:
        mutex zonas[5];
        mutex output;
        vector<thread> drones;
    public:
        void simulate();
        void flight(int drone);
        ~DronCamp() = default;
};