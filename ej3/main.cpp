#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <random>
#include <queue>
#include <mutex>
#include <condition_variable>
#define NUM_SENSORS 3
#define NUM_ROBOTS 3

using namespace std;

struct Tarea {
    int idSensor;
    int idTarea;
    string descripcion;
};

queue<Tarea> tareas;
mutex tareasMutex;
condition_variable cv;
int sensorsFinished = 0;

// Para que no se puedan repetir los id de tareas
int uuid = 0;

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

void robot() {
    while (true) {
        // Sleep de consigna
        this_thread::sleep_for(chrono::milliseconds(250));

        unique_lock<mutex> lock(tareasMutex);

        // Si no tengo tareas pero todavia los sensores no terminaron, espero (pongo el thread a dormir)
        // Manejo los supious wakeups de esta forma
        cv.wait(lock, [] { return !tareas.empty() || sensorsFinished == NUM_SENSORS; });

        // Si no hay tareas y los sensores terminaron, salgo del while (caso para que no quede un loop infinito)
        if (tareas.empty() && sensorsFinished == NUM_SENSORS) break;

        // Hago la task de este actor
        Tarea current_task = tareas.front();
        tareas.pop();
        cout << "Eliminando la tarea: " << current_task.descripcion << endl;
        
        // Desbloqueo el mutex para que el sensor pueda pushear una nueva tarea y otro robot pueda hacer su task
        lock.unlock();
    }
}

void sensor(int id) {
    int cantTareas = getRandom(1, 5);

    for (int i = 0; i < cantTareas; i++) {
        // Sleep de consigna
        this_thread::sleep_for(chrono::milliseconds(175));

        unique_lock<mutex> lock(tareasMutex);

        // Hago la task de este actor
        int idTarea = uuid++;
        string descripcion = "Tarea " + to_string(idTarea);
        tareas.push({id, idTarea, descripcion});
        cout << "Pushee tarea " << descripcion << endl;

        // Despierto a todos los threads que estan esperando por una tarea
        cv.notify_one();
    }

    // Updateo el sensorFinished protegido por el lock por si hay algun race condition
    unique_lock<mutex> lock(tareasMutex);
    sensorsFinished++;
    cv.notify_all();
}

int main() {
    vector<thread> sensors;
    vector<thread> robots;

    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors.push_back(thread(&sensor, i));
    }

    for (int i = 0; i < NUM_ROBOTS; i++) {
        robots.push_back(thread(&robot));
    }

    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i].join();
    }

    for (int i = 0; i < NUM_ROBOTS; i++) {
        robots[i].join();
    }

    return 0;
}