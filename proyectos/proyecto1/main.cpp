#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "channel.h"
#include <pthread.h>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>

using namespace std;

/**
 * Mutex para sincronizar la salida en la consola.
 */
pthread_mutex_t mtx;

/**
 * Definir el total de hilos a usar como el número de núcleos de la CPU disponibles.
 */
#define TOTAL_THREADS sysconf(_SC_NPROCESSORS_ONLN)

/**
 * Mapa global que almacenará el recuento de palabras procesadas por todos los hilos.
 */
unordered_map<string, int> global;

/**
 * Estructura para pasar los argumentos a los hilos, que principalmente incluye el canal y el recuento de palabras.
 */
struct threadData {
    /**
     * Puntero al canal de entrada de texto.
     */
    Channel<string> *lineas;

    /**
     * Puntero al mapa de recuento de palabras del hilo.
     */
    unordered_map<string, int> *wordCount;
};


// -------------------------------------------------------------------------------------------------------------------------------------//
// ------------------------------------------------------------------- METODOS ---------------------------------------------------------//
// -------------------------------------------------------------------------------------------------------------------------------------//
/**
 * Combina un hash local con un hash global, sumando los recuentos de palabras comunes
 * y agregando nuevas palabras al hash global.
 * 
 * @param global El hash global al que se agregarán las palabras y recuentos del hash local.
 * @param wordCount El hash local que se combinará con el hash global.
 */
void mergeHash(unordered_map<string, int>& global, const unordered_map<string, int>& wordCount) {
    // Iterar sobre el hash local
    for (const auto& pair : wordCount) {
        // Si la palabra ya existe en el hash global, sumar su recuento
        if (global.find(pair.first) != global.end()) {
            global[pair.first] += pair.second;
        } else {
            // Si la palabra no existe en el hash global, agregarla
            global[pair.first] = pair.second;
        }
    }
}

/**
 * Combina dos subarreglos ordenados en uno solo.
 * 
 * @param A El arreglo original que contiene los dos subarreglos ordenados que se combinarán.
 * @param izquierda El índice de inicio del primer subarreglo.
 * @param divido El índice final del primer subarreglo y el inicio del segundo subarreglo.
 * @param derecha El índice final del segundo subarreglo.
 */
void merge(vector<string>& A, int izquierda, int divido, int derecha){
    // Calcular los tamaños de los dos subarreglos a fusionar
    int tamano1 = divido - izquierda + 1;
    int tamano2 = derecha - divido;
    // Arreglos temporales para almacenar los elementos de los subarreglos
    vector<string> arreglo1(tamano1), arreglo2(tamano2);

    // Se copian los elementos de los subarreglos originales en los arreglos temporales
    for (int i = 0; i < tamano1; i++){
        arreglo1[i] = A[izquierda + i];
    } // Fin for

    for (int j = 0; j < tamano2; j++){
        arreglo2[j] = A[divido + 1 + j];
    } // Fin for

    // Fusionar los dos arreglos temporales en el arreglo original
    int i = 0;
    int j = 0;
    int k = izquierda;

    while (i < tamano1 && j < tamano2){
        if (arreglo1[i] <= arreglo2[j]){
            A[k] = arreglo1[i];
            i++;
        }else{
            A[k] = arreglo2[j];
            j++;
        } // Fin if
        k++;
    } // Fin while

    // Copiar los elementos restantes del primer subarreglo, si hay
    while (i < tamano1){
        A[k] = arreglo1[i];
        i++;
        k++;
    } // Fin while

    // Se copian los elementos restantes del segundo subarreglo, si hay
    while (j < tamano2){
        A[k] = arreglo2[j];
        j++;
        k++;
    } // Fin while
} // Fin merge


/**
 * Divide recursivamente el arreglo en subarreglos más pequeños, hasta que cada subarreglo
 * tenga uno o ningún elemento. Luego, combina los subarreglos ordenados de manera consecutiva
 * utilizando la función merge.
 * 
 * @param A El arreglo que se va a ordenar.
 * @param inicio El índice de inicio del subarreglo que se va a ordenar.
 * @param final El índice final del subarreglo que se va a ordenar.
 */
void mergeRecursivo(vector<string>& A, int inicio, int final){
    // Verifica si el índice de inicio es menor que el índice final, indicando que hay elementos en el subarreglo a ordenar
    if (inicio < final){
        // Se calcula el punto medio del subarreglo
        int divido = inicio + (final - inicio) / 2;
        // Ordenar la primera mitad del subarreglo
        mergeRecursivo(A, inicio, divido);
        // Ordenar la segunda mitad del subarreglo
        mergeRecursivo(A, divido + 1, final);
        // Merge para combinar las dos mitades ordenadas del subarreglo
        merge(A, inicio, divido, final);
    } // Fin if
} // Fin mergeRecursivo


/**
 * Algoritmo de ordenamiento Merge Sort.
 * 
 * @param A El arreglo que se va a ordenar.
 */
void mergesort(vector<string>& A){
    // Llama al método mergeRecursivo para ordenar el arreglo A
    mergeRecursivo(A, 0, A.size() - 1);
} // Fin mergesort


/**
 * Función para extraer las claves de una tabla hash y almacenarlas en un vector.
 * 
 * @param wordCount La tabla hash de la que se extraerán las claves.
 * @return Un vector que contiene todas las claves de la tabla hash.
 */
vector<string> obtenerClaves(const unordered_map<string, int>& wordCount) {
    // Inicializa un vector para almacenar las claves
    vector<string> claves;
    
    // Itera sobre cada par (clave, valor) en la tabla hash
    for (const auto& par : wordCount) {
        // Agrega la clave actual al vector de claves
        claves.push_back(par.first);
    }
    
    // Devuelve el vector que contiene todas las claves
    return claves;
}

/**
 * Función que cuenta las palabras en las líneas recibidas de un canal de entrada.
 * 
 * @param threadDataPointer Puntero genérico que apunta a la estructura de datos del hilo.
 * @return NULL al finalizar la ejecución del hilo.
 */
void *countWords(void *threadDataPointer){ 
    // Recibe el puntero de datos del hilo y lo convierte al tipo correcto
    struct threadData *data = (struct threadData *)threadDataPointer;

    // Obtiene el canal de entrada y la tabla de conteo de palabras del hilo
    Channel<string> *lineas = data->lineas;
    unordered_map<string, int> *wordCount = data->wordCount;

    // Bucle principal del hilo para procesar líneas
    while (true) {
        // Bloquea el mutex antes de verificar el canal de entrada
        pthread_mutex_lock(&mtx);
        // Verifica si el canal está cerrado y no hay más datos, luego desbloquea el mutex y sale del bucle
        if (lineas->isEmpty() && lineas->isClosed()) {
            pthread_mutex_unlock(&mtx); // Desbloquea el mutex antes de salir
            break;
        }

        // Recibe una línea del canal de entrada y luego desbloquea el mutex
        auto valorGenerico = lineas->receiveChannel();
        pthread_mutex_unlock(&mtx); // Desbloquea el mutex después de recibir el valor

        // Procesa la línea recibida
        string linea = *valorGenerico;
        istringstream iss(linea);
        string word;
        // Divide la línea en palabras y las cuenta
        while (iss >> word) {
            (*wordCount)[word]++;
        }
    } // Fin del bucle while

    // Finaliza la ejecución del hilo y devuelve NULL
    pthread_exit(NULL);
} // Fin de la función countWords


/**
 * Función productor que envía líneas de texto convertidas a minúsculas a un canal de entrada.
 * 
 * @param canalStrings Puntero al canal de entrada donde se envían las líneas de texto.
 */
void productor(Channel<string> *canalStrings) {
    // Mensaje de instrucción para el usuario
    cerr << "\n\tIngrese el texto. Ctrl+D para finalizar.\n" << endl;

    // Bucle para leer líneas de texto de la entrada estándar
    string linea;
    while (getline(cin, linea)) {
        // Convertir la línea a minúsculas usando transform y luego enviarla al canal de entrada
        transform(linea.begin(), linea.end(), linea.begin(), ::tolower);
        canalStrings->sendChannel(linea);
    } // Fin del bucle while

    // Cerrar el canal cuando el productor termina de enviar líneas de texto
    canalStrings->close();
} // Fin de la función productor


// ----------------------------------------------------------------------------------------------------------------------------------//
// ------------------------------------------------------------------- MAIN ---------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------------------------------------//

/**
 * Función principal del programa.
 */
int main() {
    // Definir el número de hilos a utilizar según la cantidad de núcleos de la CPU disponibles
    pthread_t threads[TOTAL_THREADS];

    // Inicialización del Mutex para evitar errores de sincronización
    pthread_mutex_init(&mtx, NULL);

    // Inicialización del canal de entrada de texto
    Channel<string> canalStrings;

    // Arreglo de estructuras para almacenar los datos de cada hilo
    threadData info[TOTAL_THREADS];

    // Arreglo de mapas para almacenar el recuento de palabras de cada hilo
    unordered_map<string, int> wordCounts[TOTAL_THREADS];

    // Creación de hilos para procesar el texto
    for (int i = 0; i < TOTAL_THREADS; i++) {
        info[i].lineas = &canalStrings;
        info[i].wordCount = &wordCounts[i];

        if (pthread_create(&threads[i], NULL, countWords, &info[i]) != 0) {
            perror("Error creating thread\n");
            return 1;
        } // End if
    } // End for

    // Llamada a la función productor para ingresar el texto al canal
    productor(&canalStrings);

    // Espera a que todos los hilos terminen su ejecución y fusiona los resultados
    for (int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        
        // Fusionar el recuento de palabras del hilo i con el recuento global
        pthread_mutex_lock(&mtx);
        mergeHash(global, wordCounts[i]);
        pthread_mutex_unlock(&mtx);
    }

    // Obtener las claves del mapa global
    vector<string> claves = obtenerClaves(global);
    // Ordenar las claves alfabéticamente
    mergesort(claves);
    // Mostrar las claves ordenadas con sus frecuencias
    cerr << "\n\nPalabras ordenadas alfabéticamente:\n";
    for (const string& clave : claves) {
        cout << clave << ": " << global[clave] << "\n";
    }

    // Destrucción del Mutex
    pthread_mutex_destroy(&mtx);
    return 0;
}