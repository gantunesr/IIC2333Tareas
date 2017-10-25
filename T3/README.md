# Tarea 3

| Nombre          | Número de Alumno |
|-----------------|------------------|
| Gustavo Antunes | 1363514J         |
| Cristián Cortés | 14634627         |

## Parte I

La modelación de este simulación fue hecha en base a las siguientes estructuras:

* Unidad TLB (_TLBUnit_): Unidad mínima de la TLB;
* Página (_Page_) : Unidad mínima de la tabla de páginas;
* MMU: Unidad que almacena la información de la simulación, como la cantidad de tlb hits y page faults;
* Dirección (_Adress_): Estructura que almacena el número de página o el número de frame y el offset.

El algoritmo se ejecuta en un loop infinito hasta que recibe una señal de interrupción (_Crtl + C_), este recibe la política de swapping y luego empieza a llenar la TLB y tabla de páginas con las direcciones correspondientes.

## Parte II
* Para el scheduler *scan* y *c-look* decidimos ordenar la *queue* de cilindros a leer con un algoritmo predeterminado de C - *qsort()* - Esto por motivo de que la prioridad de la tarea no era generar un algoritmo eficiente de ordenación.
* El motivo de ordenar la *queue* es porque de esta manera después solo obtenemos el índice del elemento que viene después del *head*. Dado esto, solo debemos recorrer la queue en el orden respectivo de cada scheduler.
* Consideramos que el brazo siempre venia moviendose hacia los cilindros más grandes, incluso para el *fcfs*
* *scan* considera llegar hasta el ultimo cilindro siempre
