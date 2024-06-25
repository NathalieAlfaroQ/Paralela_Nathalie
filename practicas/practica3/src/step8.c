/*
 * Autovectorizacion
 * 
 * Compilar: gcc -o step8 step8.c -O3 -ftree-vectorize -fopt-info-vec-optimized
 * Ejecutar: ./step8
 *
 * Compile en gcc:
 * -O0 (o cero)
 * -O3 -ftree-vectorize -fopt-info-vec-optimized
 * 
 * step8.c:23:21: optimized: loop vectorized using 16 byte vectors
 * step8.c:23:21: optimized:  loop versioned for vectorization because of possible aliasing
 * step8.c:23:21: optimized: loop vectorized using 8 byte vectors
 * step8.c:23:21: optimized: loop vectorized using 16 byte vectors
 * 
 * Guarde los dos ejecutables. Luego, decompílelos usando objdump -d
 * y reporte las diferencias.
 * 
 * step8.c:29:21: optimized: loop vectorized using 16 byte vectors
 * step8.c:29:21: optimized:  loop versioned for vectorization because of possible aliasing
 * step8.c:29:21: optimized: loop vectorized using 8 byte vectors
 * step8.c:29:21: optimized: loop vectorized using 16 byte vectors
 * /usr/bin/ld: cannot find objdump: No such file or directory
 * collect2: error: ld returned 1 exit status
 * 
 * Si compila los ejercicios 6 y 7 con los flags de optimización
 * sugeridos en este ejercicio: 
 * ¿el compilador logra optimizar mejor la versión “tradicional”?
 * Con -03 si
 */

// Bibliotecas
#include <stdio.h>

void add_vectors(int *a, int *b, int *result, int n) {
  for (int i = 0; i < n; i++) {
    result[i] = a[i] + b[i];
  } // End for
} // End add_vectors

int main() {
  int a[4] = {1, 2, 3, 4};
  int b[4] = {5, 6, 7, 8};
  int result[4];

  add_vectors(a, b, result, 4);

  for (int i = 0; i < 4; i++) {
    printf("%d ", result[i]);
  } // End for

  // Fin del programa
  return 0;
} // End main