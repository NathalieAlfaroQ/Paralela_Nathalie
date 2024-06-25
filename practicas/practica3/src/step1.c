/*
Instrucciones SIMD (Single Instruction, Multiple Data)
utilizando intrinsics y explorar la autovectorización
con gcc en Linux

Sume dos vectores de enteros utilizando intrinsics SIMD, en step1.c

Investigue las instrucciones _mm_loadu_si128, _mm_add_epi32 y
_mm_storeu_si128, en el archivo Markdown

Compilar: gcc -o step1 step1.c
Ejecutar: ./step1
*/

// Bibliotecas
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  /*
   * Tipo de dato: int32_t
   * Es un entero con signo de 32 bits (4 bytes)
   * Garantiza que la variable tenga un tamaño específico de 32 bits
   * en cualquier plataforma, lo que es útil para la portabilidad del código.
   */
  // Arreglos con valores
  int32_t arregloA[4] = {1, 2, 3, 4};
  int32_t arregloB[4] = {5, 6, 7, 8};

  // Arreglo para guardar la suma de los dos vectores
  int32_t resultadoSumar[4];

  // Cargo al registro el arreglo A y B a un registro de __m128i
  __m128i vectorA = _mm_loadu_si128((__m128i *)arregloA);
  __m128i vectorB = _mm_loadu_si128((__m128i *)arregloB);

  // Suma cada elemento del vector y almacena el
  // resultado en un registro tipo __m128i
  __m128i sumaVectores = _mm_add_epi32(vectorA, vectorB);

  // Almacena un bloque de 128 bits desde un registro SIMD
  _mm_storeu_si128((__m128i *)resultadoSumar, sumaVectores);

  /*
   * Tipo de dato sin signo (unsigned) y su tamaño varia según la arquitectura
   * del sistema: En sistemas de 32 bits, size_t generalmente es de 32 bits (4
   * bytes). En sistemas de 64 bits, size_t generalmente es de 64 bits (8
   * bytes). Representa tamaños de objetos en memoria Usualmente se usa para
   * recorrer arrays o manipular memoria
   */
  // Ciclo para imprimir el arreglo que contiene la suma
  for (size_t i = 0; i < 4; i++) {
    printf("%d ", resultadoSumar[i]);
  } // End for

  printf("\n");

  // Fin del programa
  return 0;
} // End main