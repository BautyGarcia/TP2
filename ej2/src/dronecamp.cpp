#include "../include/dronecamp.hpp"

void DronCamp::flight(int drone) {
    int left = drone; // la zona de la izquierda va a ser el mismo indice que el dron
    int right = (drone + 1) % 5; // la zona de la derecha solamente le sumo 1 al indice y manejo el caso del borde

    // bloqueo los outputs porque sino se me mezclaban los mensajes
    output.lock();
    cout << "Dron " << drone << " esperando para despegar..." << endl;
    output.unlock();

    // ocupo las zonas
    zonas[left].lock();
    zonas[right].lock();

    output.lock();
    cout << "Dron " << drone << " despegando..." << endl;
    output.unlock();

    this_thread::sleep_for(chrono::seconds(5));

    output.lock();
    cout << "Dron " << drone << " alcanzó altura de 10m" << endl;
    output.unlock();

    // desocupo las zonas
    zonas[left].unlock();
    zonas[right].unlock();
}

void DronCamp::simulate() {
    // Creo y joineo los threads en ciclos distintos porque si lo hago
    // inmediatamente despues en realidad no seria concurrencia 
    for (size_t i = 0; i < 5; i++) {
        drones.push_back(thread(&DronCamp::flight, this, i));
    }

    for (size_t i = 0; i < 5; i++) {
        drones[i].join();
    } 
}