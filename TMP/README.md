# TAREA 1 - Gustavo Antunes | Cristian Cortés

## Estructuras

El algoritmo utiliza las siguientes estructuras:

* Process: Estructura que representa un proceso;
* Queue: Estructura que representa una cola. Esta es utilizada para modelar las diferentes colas de los procesos, estas colas se hacen en base al estado actual de los procesos (WAITING, READY o DEAD);
* Seqqueue: Estructura que representa una cola de secuencia. Esta es utilizada para modelar la secuencia de tiempo de los procesos (A_1, B_1, A_2, B_2, ..., B_N-1, A_N).

## Observaciones

* Si el Q_k de un proceso que con valor 0 o negativo, este pasará a tomar el valor de 1. Dado que si no el proceso no terminaria nunca de ejecutarse.
* Si un proceso es interrumpido (Ctrl+C), no se calcula su *turnaround_time* dado que este nunca finaliza. Por defecto su valor será 0.
