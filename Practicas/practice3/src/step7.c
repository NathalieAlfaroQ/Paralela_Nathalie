/*
Multiplicación escalar
Implemente la multiplicación por un escalar. Cree tanto una versión que use SIMD como
una tradicional. Ambas funciones deben recibir un array de int32_t (importen limits.h) y un
size_t con el tamaño del array. Utilicen _mm_loadu_si128 para cargar los datos,
_mm_set1_epi32 para guardar el escalar en cada posición del registro, _mm_mullo_epi32
para realizar la multiplicación y finalmente _mm_storeu_si128 para guardar el resultado al
array.
Compile el programa y luego decompílelo usando objdump -d. Observe las diferencias en
el código generado. ¿El compilador logró optimizar la versión “tradicional”?
*/