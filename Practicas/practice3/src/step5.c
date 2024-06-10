/*
Realice la multiplicación de dos matrices 2x2 utilizando intrinsics SIMD.

Compilar: gcc -o step5 step5.c -msse4.1
Ejecutar: ./step5
*/

// Bibliotecas
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
/*
int main() {
  int32_t A[2][2] = {{1, 2}, {3, 4}};
  int32_t B[2][2] = {{5, 6}, {7, 8}};
  int32_t C[2][2];

  __m128i row1 = _mm_set_epi32(0, 0, A[0][1], A[0][0]);
  __m128i row2 = _mm_set_epi32(0, 0, A[1][1], A[1][0]);
  __m128i col1 = _mm_set_epi32(0, 0, B[1][0], B[0][0]);
  __m128i col2 = _mm_set_epi32(0, 0, B[1][1], B[0][1]);

  __m128i mul1 = _mm_mullo_epi32(row1, col1);
  __m128i mul2 = _mm_mullo_epi32(row1, col2);
  __m128i mul3 = _mm_mullo_epi32(row2, col1);
  __m128i mul4 = _mm_mullo_epi32(row2, col2);

  C[0][0] = _mm_extract_epi32(mul1, 0) + _mm_extract_epi32(mul1, 1);
  C[0][1] = _mm_extract_epi32(mul2, 0) + _mm_extract_epi32(mul2, 1);
  C[1][0] = _mm_extract_epi32(mul3, 0) + _mm_extract_epi32(mul3, 1);
  C[1][1] = _mm_extract_epi32(mul4, 0) + _mm_extract_epi32(mul4, 1);

  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      printf("%d ", C[i][j]);
    } // End for
    printf("\n");
  } // End for

  // Fin del programa
  return 0;
} // End main
*/

int main() {
    int32_t A[2][2] = { {1, 2}, {3, 4} };
    int32_t B[2][2] = { {5, 6}, {7, 8} };
    int32_t C[2][2];

    // Cargar filas de A en registros SIMD
    __m128i row1 = _mm_loadu_si128((__m128i *)A[0]);
    __m128i row2 = _mm_loadu_si128((__m128i *)A[1]);

    // Cargar columnas de B en registros SIMD
    __m128i col1 = _mm_set_epi32(B[1][0], B[0][0], B[1][0], B[0][0]);
    __m128i col2 = _mm_set_epi32(B[1][1], B[0][1], B[1][1], B[0][1]);

    // Multiplicar y sumar productos parciales para cada elemento de C
    __m128i prod1 = _mm_mullo_epi32(row1, col1);
    __m128i prod2 = _mm_mullo_epi32(row1, col2);
    __m128i prod3 = _mm_mullo_epi32(row2, col1);
    __m128i prod4 = _mm_mullo_epi32(row2, col2);

    // Calcular elementos finales de C
    C[0][0] = _mm_extract_epi32(prod1, 0) + _mm_extract_epi32(prod1, 1);
    C[0][1] = _mm_extract_epi32(prod2, 0) + _mm_extract_epi32(prod2, 1);
    C[1][0] = _mm_extract_epi32(prod3, 0) + _mm_extract_epi32(prod3, 1);
    C[1][1] = _mm_extract_epi32(prod4, 0) + _mm_extract_epi32(prod4, 1);

    // Imprimir la nueva matriz
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            printf("%d ", C[i][j]);
        } // End for
        printf("\n");
    } // End for

    // Fin del programa
    return 0;
} // End main
