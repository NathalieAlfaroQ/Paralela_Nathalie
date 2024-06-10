La memoria no alineada se refiere a un acceso a memoria en el que la dirección no está alineada según el tamaño del tipo de dato que se está accediendo. En contraste, la memoria alineada significa que las direcciones de memoria están alineadas a los límites que son múltiplos del tamaño de los datos.

Ejemplo de Memoria Alineada
Para entender la memoria alineada, considera que tienes un tipo de dato de 4 bytes (como un entero de 32 bits). La memoria alineada para este tipo de dato significa que cualquier acceso a este tipo de dato debe comenzar en una dirección que sea múltiplo de 4 (por ejemplo, 0x0004, 0x0008, 0x000C, etc.). De manera similar, un tipo de dato de 16 bytes debe estar alineado a una dirección que sea múltiplo de 16.

Ejemplo de Memoria No Alineada
Si tienes un acceso a un dato de 4 bytes que empieza en una dirección de memoria que no es múltiplo de 4 (por ejemplo, 0x0001, 0x0002, 0x0003, etc.), entonces se dice que es memoria no alineada.

Importancia de la Alineación de Memoria
Eficiencia del Procesador:

Los procesadores modernos están optimizados para acceder a datos alineados. El acceso a memoria alineada puede ser más rápido porque puede realizarse en una sola operación de memoria.
Compatibilidad de Instrucciones SIMD:

Muchas instrucciones SIMD requieren que los datos estén alineados a ciertos límites (por ejemplo, 16 bytes para registros de 128 bits) para funcionar correctamente. El acceso no alineado puede causar penalizaciones de rendimiento o, en algunos casos, errores.
Uso de Instrucciones SIMD con Memoria No Alineada
Las instrucciones SIMD, como _mm_loadu_si128 y _mm_storeu_si128, están diseñadas para trabajar con memoria no alineada. Esto permite que los programadores accedan a datos sin preocuparse por la alineación de los mismos, aunque con una posible penalización en el rendimiento.


////
_mm_loadu_si128 carga un bloque de 128 bits de memoria no alineada en un
registro SIMD.
__m128i _mm_loadu_si128 (__m128i const* mem_addr);
carga un bloque de 128 bits (16 bytes) desde la dirección mem_addr en un
registro __m128i. La u en loadu significa "unaligned" (no alineado), lo que
indica que la dirección de memoria no necesita estar alineada a un límite de 16
bytes.
__m128i data = _mm_loadu_si128((__m128i*)ptr);

_mm_add_epi32 suma los elementos de 32 bits (entero con signo) en dos registros
SIMD.
__m128i _mm_add_epi32 (__m128i a, __m128i b);
suma componente a componente de enteros de 32 bits de los registros a y b, y
almacena el resultado en un registro __m128i. Cada entero de 32 bits en a se
suma al correspondiente entero de 32 bits en b.
__m128i result = _mm_add_epi32(a, b);

_mm_storeu_si128 almacena un bloque de 128 bits desde un registro SIMD en
memoria no alineada. void _mm_storeu_si128 (__m128i* mem_addr, __m128i a);
Almacena los 128 bits (16 bytes) del registro a en la dirección de memoria
mem_addr. Similar a _mm_loadu_si128, la u indica que la dirección de memoria no
necesita estar alineada a un límite de 16 bytes. 
_mm_storeu_si128((__m128i*)ptr,
data);

_mm_sub_epi32 realiza la resta de dos registros __m128i que contienen cuatro enteros de 32 bits cada uno. La operación se realiza en paralelo para cada par correspondiente de enteros de los dos registros de entrada.

__m128i _mm_sub_epi32(__m128i a, __m128i b);

Cada elemento de b se resta del elemento correspondiente de a. Los resultados se almacenan en un nuevo registro __m128i.

