#ifndef chanel_h
#define chanel_h
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <pthread.h>

//Uso de plantillas para poder usar cualquier tipo de dato
template<typename T>

//Implementacion del canal
class Channel {

    public:
        //Enviar valores al canal de los valores que se definan al instanciarla
        void sendChannel(T valorGenerico) {
            //Crear un candado
            //INVESTIGAR
            std::lock_guard<std::mutex> lock(mtx);
            //Pusheo del valor a la cola
            colaGenerica.push(valorGenerico);
            //notify_one() despierta a uno de los hilos que está esperando en la condición variable cond_var.
            cond_var.notify_one();

        }

        //Recibir(pop) los valores de la cola
        //"optional" una utilidad de la biblioteca estándar de C++ que representa un objeto que contiene un valor de tipo T o no contiene valor (está vacío).
        std::optional<T> receiveChannel() {

            // -[this] Captura el puntero this de la instancia actual de la clase, para que pueda acceder a la queue.
            // -[this] { return !queue.empty(); } expresión lambda que define la condición que se debe cumplir para que el hilo deje de esperar.
            //      Si la cola está vacía, devuelve false y el hilo sigue esperando.
            // -lock Su propósito principal es gestionar el mutex asociado (en este caso, mtx) para asegurar que el acceso a la cola queue sea 
            //      seguro y sincronizado entre los hilos

            //INVESTIGAR
            std::unique_lock<std::mutex> lock(mtx);
            cond_var.wait(lock, [this] { return !colaGenerica.empty(); });
            //Si el canal está cerrado y la cola está vacía, devolver un optional vacío
            if (closed && colaGenerica.empty()) {
                return std::nullopt;
            }
            T valorGenerico = colaGenerica.front();
            colaGenerica.pop();
            return valorGenerico;

        }

        // Método para verificar si la cola está vacía
        bool isEmpty() {
            std::lock_guard<std::mutex> lock(mtx);
            return colaGenerica.empty();
        }

        bool isClosed() {
            std::lock_guard<std::mutex> lock(mtx);
            return closed;
        }

        // Método para cerrar el canal
        void close() {
            std::lock_guard<std::mutex> lock(mtx);
            closed = true;
            cond_var.notify_all();
        }

    private:
        //Mutex
        std::mutex mtx; // Utilizar std::mutex
        //Condicion de variable
        std::condition_variable cond_var;
        //La cola con cualquier tipo de dato
        std::queue<T> colaGenerica;
        //Indica si el canal está cerrado
        bool closed = false;
};


#endif