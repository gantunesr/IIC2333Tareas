# TAREA 1 - Gustavo Antunes | Cristian Cortés

## Estructuras

El algoritmo utiliza las siguientes estructuras:

* Process: Estructura que representa un proceso;
* Queue: Estructura que representa una cola. Esta es utilizada para modelar las diferentes colas de los procesos, estas colas se hacen en base al estado actual de los procesos (WAITING o READY);
* Seqqueue: Estructura que representa una cola de secuencia. Esta es uitizada para modelar la secuencia de tiempo de los procesos (A_1, B_1, A_2, B_2, ..., B_N-1, A_N).

## Observaciones

* Si el usuario intenta ingresar un quantum cuyo valor es igual a cero, el algoritmo lo cambiará a uno;
