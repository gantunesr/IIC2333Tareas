# Tarea 2

| Nombre          | Número de Alumno |
|-----------------|------------------|
| Gustavo Antunes | 1363514J         |
| Cristián Cortés | 14634627         |

## Lifegame

### Estructuras
Este programa se basa en las siguientes estructuras:
* **Matrix**: La matriz de que posee todas las bacterias del juego de la vida; cada bacteria se representa por un 0 si esta muerta y un 1 si esta viva.
* **ThreadBase**: Estructura utilizada para darle argumentos a la función _lifegame_, la cual es ejecutada por distintos threads. En particular, los argumentos requeridos en cada thread son dos arreglos, uno de filas y otro de columnas, que posee los indices de las casillas de debe revisar el thread.

### Flujo del programa
El programa funciona con dos matrices, la matriz principal y la matriz futura. En cada iteración, se revisa cada celda de la matriz principal para ver si en ese casillera una bacteria nace, vive o muere. El resultado de lo anterior es guardado en el **mismo casillero**, pero en la **matriz futura**. Una vez recorrida la matriz, se principalizan los valores de matriz principal con los valores guardados en matriz futura.\
Esta situación se repite hasta terminar las iteraciones entregadas en el input.

### Sincronización
Para manejar la sincronización de los _threads_ dentro del programa se utilizó join en dos partes del programa. Primero, se crean los *threads* para que el cada uno verifique que pasa con cada celda de la matriz y guarde su resultado en matriz futura. Aquí se utiliza *join* para asegurarnos que antes de principalizar la matriz principal, todas las casillas hayan sido revisadas. De igual manera, se crean *threads* para principalizar la matriz principal con los valores en la matriz futura. Acá se vuelve a usar *join* para asegurarnos que la matriz principal haya sido completamente actualizada antes de empezar a calcular sus cambios para la futura iteración.

## MyShell

### Algoritmo

* **Suposición 1**: El algoritmo funciona con la suposición que los comandos serán escritos de manera correcta, en caso contrario este terminará su ejecución.

#### I - Procesamiento

El algoritmo posee las funciones (buildins) en un arreglo. Cuando se ingresa un comando,
se busca entre los argumentos un buildin. Si lo encuentre, se realiza una comparación de strings y se ejecuta el comando ingresado junto con los demás argumentos, los cuales son parseados y almacenados en otro arrelo. En caso contrario, se pide un nuevo comando.

#### II - Proceso

Para la creación de los procesos, se usa las funciones _fork()_, _execvp()_, _perror()_, _exit()_ y _waitpid()_. Además, se utiliza las funciones _WIFSIGNALED()_ y _WIFEXITED()_ de la siguiente manenra:

* _WIFSIGNALED()_ = 1, si child termino por signal.
* _WIFEXITED()_ =  1, si child termino normalmente (con _exit()_, por ejemplo).

### Observaciones

* No se pude completar el último punto del enunciado ("Si el último argumento es &N, el proceso debe ser ejecutado N veces en paralelo (esto significa que pueden estar en estado ready simultánamente).")
