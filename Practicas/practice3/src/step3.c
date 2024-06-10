/*
 * Calcule el producto punto de dos vectores de enteros usando SIMD, en step3.c
 *
 * Investigue el uso de _mm_mullo_epi32, en el archivo Markdown
 *
 * La reducción final de cada elemento de 32 bits deben hacerla con
 * código en C típico, ya que no existe una instrucción _mm_reduce_add_epi32 (la
 * más parecida requiere AVX512).
 *
 * Compilar: gcc -msse4.1 -o step3 step3.c
 * Ejecutar: ./step3
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

  // Arreglo para guardar el producto de los dos vectores
  int32_t resultadoProducto[4];

  // Cargo al registro el arreglo A y B a un registro de __m128i
  __m128i vectorA = _mm_loadu_si128((__m128i *)arregloA);
  __m128i vectorB = _mm_loadu_si128((__m128i *)arregloB);

  // Multiplica cada elemento del vector y almacena el
  // resultado en un registro tipo __m128i
  __m128i productoVectores = _mm_mullo_epi32(vectorA, vectorB);

  // Almacena un bloque de 128 bits desde un registro SIMD
  _mm_storeu_si128((__m128i *)resultadoProducto, productoVectores);

  // Sumar los productos
  int32_t productoPunto = 0;
  for (size_t i = 0; i < 4; i++) {
    productoPunto += resultadoProducto[i];
  } // End for

  // Imprimir el resultado del producto punto
  printf("Producto punto: %d\n", productoPunto);

  // Fin del programa
  return 0;
} // End main
