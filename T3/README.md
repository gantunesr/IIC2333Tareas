# Tarea 3

| Nombre          | Número de Alumno |
|-----------------|------------------|
| Gustavo Antunes | 1363514J         |
| Cristián Cortés | 14634627         |

## Parte I

## Parte II
* Para el scheduler *scan* y *c-look* decidimos ordenar la *queue* de cilindros a leer con un algoritmo predeterminado de C - *qsort()* - Esto por motivo de que la prioridad de la tarea no era generar un algoritmo eficiente de ordenación.
* El motivo de ordenar la *queue* es porque de esta manera después solo obtenemos el índice del elemento que viene después del *head*. Dado esto, solo debemos recorrer la queue en el orden respectivo de cada scheduler.
* Consideramos que el brazo siempre venia moviendose hacia los cilindros más grandes, incluso para el *fcfs*
* *scan* considera llegar hasta el ultimo cilindro siempre
