/*
Compilar: gcc -o step7 step7.c -msse4.1
Ejecutar: ./step7

Implemente la multiplicación por un escalar. Cree tanto una versión que use SIMD
como una tradicional. Ambas funciones deben recibir un array de int32_t
(importen limits.h) y un size_t con el tamaño del array. Utilicen
_mm_loadu_si128 para cargar los datos, _mm_set1_epi32 para guardar el escalar en
cada posición del registro, _mm_mullo_epi32 para realizar la multiplicación y
finalmente _mm_storeu_si128 para guardar el resultado al array.

Compile el programa y luego decompílelo usando objdump -d. Observe las
diferencias en el código generado. ¿El compilador logró optimizar la versión
“tradicional”?
*/

// Bibliotecas
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <immintrin.h>

// Esta funcion recorre cada elemento del arreglo y
// lo multiplica por un escalar
void multiTradicional(int32_t *arreglo, size_t tamano, int32_t escalar) {
  for (size_t i = 0; i < tamano; ++i) {
    arreglo[i] *= escalar;
  } // End for
} // End multiTradicional

void multiSIMD(int32_t *arreglo, size_t tamano, int32_t escalar) {
  // Cargar el escalar
  __m128i multiplicando = _mm_set1_epi32(escalar);

  size_t i;
  for (i = 0; i + 4 <= tamano; i += 4) {
    // Cargar el arreglo
    __m128i vector = _mm_loadu_si128((__m128i *)&arreglo[i]);

    // Multiplicar
    __m128i resultado = _mm_mullo_epi32(vector, multiplicando);

    // Guardar el resultado en el arreglo
    _mm_storeu_si128((__m128i *)&arreglo[i], resultado);
  } // End for

  // Procesar los que no pudieron ser procesados con SIMD
  for (; i < tamano; ++i) {
    arreglo[i] *= escalar;
  } // End for
} // End multiSIMD

int main() {
  int32_t arreglo[] = {1, 2, 3, 4, 5, 6, 7, 8};
  size_t tamano = sizeof(arreglo) / sizeof(arreglo[0]);
  int32_t escalar = 3;

  multiTradicional(arreglo, tamano, escalar);

  printf("Producto modo tradicional:\n");
  for (size_t i = 0; i < tamano; ++i) {
    printf("%d ", arreglo[i]);
  } // End for
  printf("\n");

  // Ahora para SIMD
  int32_t arregloSIMD[] = {1, 2, 3, 4, 5, 6, 7, 8};
  multiSIMD(arregloSIMD, tamano, escalar);

  printf("Producto modo SIMD:\n");
  for (size_t i = 0; i < tamano; ++i) {
    printf("%d ", arregloSIMD[i]);
  } // End for
  printf("\n");

  // Fin del programa
  return 0;
} // End main
