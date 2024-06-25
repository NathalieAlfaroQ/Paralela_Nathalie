# Proyecto Programación Concurrente y Paralela: Conteo de Palabras

## Miembros del equipo
- Mario Cordero Aguilar C22306
- Nathalie Alfaro Quesada B90221

## Partes elaboradas por cada miembro
### Mario Cordero
- Implementación de la lectura del documento y la división del trabajo entre threads.
- Implementación del canal.

### Nathalie Alfaro
- Implementación del conteo de palabras por medio de un hashtable.
- Ordenamiento alfabético de las palabras mediante el merge sort.

## Cómo usar
- Para ejecutar el programa, debe ser en una distribución Linux y debe asegurarse tener g++ y make instalado.
- Deberá abrir una terminal en esta carpeta y escribir "make". El archivo Makefile se encargaŕa del resto.

```bash
make
```

## Diseño de su programa
El programa se divide en varias etapas:
1. Lectura del documento desde la entrada estándar.
2. División del trabajo entre threads para procesar el documento.
3. Conteo de palabras y ordenamiento alfabético.
4. Spilling a disco para gestionar la memoria.
5. Presentación de los resultados.

## Problemas encontrados
Durante el desarrollo del proyecto, nos encontramos con los siguientes problemas:
- Gestión eficiente de la memoria al procesar archivos grandes.
- Coordinación entre threads para evitar condiciones de carrera.
- Cálculo del tamaño de las variables usadas en el programa
- Escritura en disco una vez sobrepasado el límite de memoria de las variables usadas en el programa.

## Fechas de evaluación
- [x] Lunes 6 de mayo: Diseño del proyecto.
- [x] 13 y 20 de mayo: Avances del progreso del proyecto.
- [x] 26 de mayo a medianoche: Entrega del proyecto.
- [x] 27 de mayo: Presentación final del proyecto.

## Evaluación
Se evaluarán los siguientes aspectos:
- Calidad del código: 25%
- Eficiencia y escalabilidad del programa: 25%
- Spilling: 10%
- Correctitud: 10%
- No hay fugas de memoria: 10%
- Documentación: 20%
