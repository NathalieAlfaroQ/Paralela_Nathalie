/*
Reducción paralela

Investigue el funcionamiento de la función _mm_hadd_epi32. Una vez que entienda
cómo funciona, implemente una función que sume todos los elementos de un array.
Igual que en el resto de la práctica, considere arrays de int32_t y reciba el
tamaño en un size_t. Sume los elementos de 4 en 4 usando _mm_add_epi32, luego
sume el resultado acumulado en el registro SSE usando la suma parcial del código
anterior. Finalmente, sume los elementos que no pudo procesar usando SIMD.

gcc -o step9 step9.c -msse4.1
./step9
*/

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Suma todos los elementos de un arreglo con SIMD
int32_t sumarSIMD(const int32_t *arreglo, size_t tamano) {
  // Inicializar el acumulador a cero
  __m128i suma = _mm_setzero_si128();

  size_t i;
  // Sumar elementos de 4 en 4
  for (i = 0; i + 4 <= tamano; i += 4) {
    __m128i valores = _mm_loadu_si128((__m128i *)&arreglo[i]);
    suma = _mm_add_epi32(suma, valores);
  } // End for

  // Sumas horizontales
  suma = _mm_hadd_epi32(suma, suma);
  suma = _mm_hadd_epi32(suma, suma);

  // Traer el resultado
  int32_t resultado = _mm_extract_epi32(suma, 0);

  // Sumar los elementos que no pudieron ser procesados con SIMD
  for (; i < tamano; ++i) {
    resultado += arreglo[i];
  } // End for

  return resultado;
} // End sumarSIMD

int main() {
  int32_t arreglo[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  size_t tamano = sizeof(arreglo) / sizeof(arreglo[0]);

  int32_t suma = sumarSIMD(arreglo, tamano);

  printf("Suma: %d\n", suma);

  // Fin del programa
  return 0;
} // End main
