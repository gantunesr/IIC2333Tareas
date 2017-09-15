#Tarea 2
*Gustavo Antunez | <inserte_numero_alumno_de_guto>\
Cristián Cortés | 14634627*

##Lifegame
####Estructuras
Este programa se basa en las siguientes estructuras:
* **Matrix**: La matriz de que posee todas las bacterias del juego de la vida; cada bacteria se representa por un 0 si esta muerta y un 1 si esta viva.
* **ThreadBase**: Estructura utilizada para darle argumentos a la función _lifegame_, la cual es ejecutada por distintos threads. En particular, los argumentos requeridos en cada thread son dos arreglos, uno de filas y otro de columnas, que posee los indices de las casillas de debe revisar el thread.

####Flujo del programa
El programa funciona con dos matrices, la matriz principal y la matriz futura. En cada iteración, se revisa cada celda de la matriz principal para ver si en ese casillera una bacteria nace, vive o muere. El resultado de lo anterior es guardado en el **mismo casillero**, pero en la **matriz futura**. Una vez recorrida la matriz, se principalizan los valores de matriz principal con los valores guardados en matriz futura.\
Esta situación se repite hasta terminar las iteraciones entregadas en el input.

####Sincronización
Para manejar la sincronización de los _threads_ dentro del programa se utilizó join en dos partes del programa. Primero, se crean los *threads* para que el cada uno verifique que pasa con cada celda de la matriz y guarde su resultado en matriz futura. Aquí se utiliza *join* para asegurarnos que antes de principalizar la matriz principal, todas las casillas hayan sido revisadas. De igual manera, se crean *threads* para principalizar la matriz principal con los valores en la matriz futura. Acá se vuelve a usar *join* para asegurarnos que la matriz principal haya sido completamente actualizada antes de empezar a calcular sus cambios para la futura iteración.

## MyShell
