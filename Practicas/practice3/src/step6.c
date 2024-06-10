/*
Mayor elemento de un array

Cree dos funciones que calculen el mayor elemento de un array de enteros de 32
bits.

Ambas funciones deben recibir un array de int32_t (importen limits.h) y un
size_t con el tamaño del array. Una impleméntela usando instrucciones SIMD (con
registros de 128 bits) y otra impleméntela usando un for tradicional. Para la
versión SIMD, considere usar _mm_set1_epi32 para setear el valor inicial de los
registros en INT32_MIN (importe limits.h), _mm_load_si128 para cargar datos del
array y _mm_max_epi32 para obtener los elementos máximos uno a uno. Luego usen
_mm_storeu_si128 para guardar el registro en un array normal (en la pila).
Finalmente, usen programación tradicional para obtener el mayor elemento tanto
del array final que crearon con _mm_storeu_si128 como de los elementos restantes
que no pudieron procesar con _mm_max_epi32.

Compilar: gcc -o step6 step6.c -msse4.1
Ejecutar: ./step6
*/

// Bibliotecas
#include <immintrin.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Esta funcion recibe arreglo de tipo int32_t y el tamano del
 * arreglo tipo size_t, su tarea es buscar en el arreglo cual
 * es el valor maximo y lo retorna
 */
int32_t maximoTradicional(const int32_t *arreglo, size_t tamano) {
  int32_t maximoValor = INT32_MIN;
  for (size_t i = 0; i < tamano; ++i) {
    if (arreglo[i] > maximoValor) {
      maximoValor = arreglo[i];
    } // End if
  }   // End for
  return maximoValor;
} // End maximoTradicional

int32_t maximoSIMD(const int32_t *arreglo, size_t tamano) {
  // Inicializar el maximo valor
  __m128i maxValores = _mm_set1_epi32(INT32_MIN);

  size_t i;
  for (i = 0; i + 4 <= tamano; i += 4) {
    // Itera en bloques de 4 elementos para cargar
    // datos en el registro SIMD
    __m128i valores = _mm_load_si128((__m128i *)&arreglo[i]);

    // Obtener el valor máximo entre los elementos
    // cargados y el valor máximo actual
    maxValores = _mm_max_epi32(maxValores, valores);
  } // End for

  // Almacenar los valores máximos encontrados en un array temporal
  int32_t maxArreglo[4];
  _mm_storeu_si128((__m128i *)maxArreglo, maxValores);

  // Obtener el mayor valor del array temporal
  int32_t maxValor = INT32_MIN;
  for (size_t j = 0; j < 4; ++j) {
    if (maxArreglo[j] > maxValor) {
      maxValor = maxArreglo[j];
    } // End if
  }   // End for

  // Procesar los que no pudieron
  // ser procesados con SIMD
  for (; i < tamano; ++i) {
    if (arreglo[i] > maxValor) {
      maxValor = arreglo[i];
    } // End if
  }   // End for

  return maxValor;
} // End maximoSIMD

int main() {
  int32_t vector[] = {1, 3, 7, 2, 8, 4, 10, 6};
  size_t tamano = sizeof(vector) / sizeof(vector[0]);

  int32_t maxTradicional = maximoTradicional(vector, tamano);
  int32_t maxSIMD = maximoSIMD(vector, tamano);

  printf("Maximo valor en modo tradicional: %d\n", maxTradicional);
  printf("Maximo valor en modo SIMD: %d\n", maxSIMD);

  // Fin del programa
  return 0;
} // End main
