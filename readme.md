### Reporte


Tarea 1: Calcular Pi usando pthreads
Curso: Programacion paralela y concurrente  


El objetivo de esta tarea es calcular el valor de π utilizando el método de Monte Carlo. Para ello, se generan puntos
aleatorios en un cuadrado de lado 1 y se cuentan cuántos de estos puntos caen dentro de un círculo de radio 1. El valor de π se puede aproximar mediante la siguiente fórmula:


        π ≈ 4 × (puntos dentro del círculo / puntos totales)


Usen pthreads en C para paralelizar el cálculo de π. El programa debe recibir como argumento la cantidad de puntos
a generar y el número de hilos a utilizar. El programa debe imprimir el valor de π calculado y el tiempo que tomó
realizar el cálculo.


 1. Implementación del programa en C, siguiendo las siguientes estrategias (un archivo por estrategia):
    -  El programa original, sin modificaciones.
    -  El programa modificado para que el cálculo de π se realice en paralelo utilizando pthreads.
        - No protejan la variable points_inside_circle con un mutex.
        - Protejan la variable points_inside_circle con un mutex dentro del ciclo for.
        - Protejan la variable points_inside_circle con un mutex fuera del ciclo for. En lugar de incrementar la variable global  points_inside_circle dentro del ciclo, actualicen una variable local y al final del ciclo incrementen la variable global.
        - Similar a 3, pero no actualicen points_inside_circle dentro de throw_darts. Retornen la cantidad de puntos dentro del círculo (usando un falso puntero) y hagan que el hilo principal incremente la variable global.
        - Usen operaciones atómicas para incrementar la variable points_inside_circle.

 2. Instrucciones detalladas de cómo compilar el programa. Si usaron un Makefile añádanlo.
 3. Prueben su programa usando Helgrind o ThreadSanitizer y describan los problemas de concurrencia detectados, porqué sucedieron y cómo los    corrigieron. Corríjanlos y vuelvan a probar.
    - Por supuesto, NO corrijan los errores en 1.2.1.

 4. Realicen pruebas con diferentes cantidades de puntos y de hilos. Muestren los resultados obtenidos y el tiempo
 de ejecución en cada caso. Pueden usar el comando time para medir el tiempo de ejecución (por ejemplo, time
 ./pi 100000000 4).


Resultados de las ejecuciones:


    Para time ./calculaPi 100000000 4   
        real    0m0.002s
        user    0m0.001s
        sys     0m0.001s
        Valor de pi usando operaciones atómicas: 3.141793

    Para time ./calculaPi 100000000 8
        Valor de pi usando operaciones atómicas: 3.141691
        real    0m0.354s
        user    0m2.519s
        sys     0m0.000s

    Para time ./calculaPi 100000000 6
        El número de puntos debe ser divisible entre el número de hilos
        real    0m0.003s
        user    0m0.002s
        sys     0m0.002s

    Para time ./calculaPi 800000000 8
        Valor de pi usando operaciones atómicas: 3.141562
        real    0m2.308s
        user    0m18.015s
        sys     0m0.021s


Para compilar el Helgrind:


```
gcc -pthread -g -o calculaPi calculaPi.c -lm
valgrind --tool=helgrind time ./calculaPi 100000000 4
```


Resultados de Helgrind:


==114948== Helgrind, a thread error detector
==114948== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==114948== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==114948== Command: time ./calculaPi 100000000 4
==114948== 
Valor de pi usando operaciones atómicas: 3.141469
1.72user 0.00system 0:00.48elapsed 354%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+957minor)pagefaults 0swaps
==114948== 
==114948== Use --history-level=approx or =none to gain increased speed, at
==114948== the cost of reduced accuracy of conflicting-access information
==114948== For lists of detected and suppressed errors, rerun with: -s
==114948== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


Resultados de Helgrind:


```
gcc -pthread -fsanitize=thread -o calculaPigcc calculaPi.c -lm
time ./calculaPi 100000000 4
```


Valor de pi usando operaciones atómicas: 3.141516

real    0m0.480s
user    0m1.794s
sys     0m0.001s