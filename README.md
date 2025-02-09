# Philosophers

## 📌 Descripción  
`philo` es una simulación del problema clásico de los filósofos comensales utilizando hilos (`pthread`) y mutexes para la sincronización de recursos.  

## 📁 Archivos a entregar  
El proyecto debe incluir los siguientes archivos en el directorio `philo/`:  
- `Makefile`  
- Archivos de encabezado (`*.h`)  
- Archivos de código fuente (`*.c`)  

## 🛠️ Especificaciones  

| Nombre del programa | `philo` |
|---------------------|---------|
| 📂 Archivos a entregar | `Makefile`, `*.h`, `*.c` (en el directorio `philo/`) |
| ⚙️ Reglas del Makefile | `NAME`, `all`, `clean`, `fclean`, `re` |
| 🚀 Argumentos | `number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]` |
| 📜 Funciones permitidas | `memset`, `printf`, `malloc`, `free`, `write`, `usleep`, `gettimeofday`, `pthread_create`, `pthread_detach`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_destroy`, `pthread_mutex_lock`, `pthread_mutex_unlock` |
| ❌ Se permite usar libft | No |
| 📌 Descripción | Filosofía concurrente con hilos y mutex |

## ⚙️ Compilación  
El `Makefile` debe contener las siguientes reglas:  
- `NAME` → Nombre del ejecutable (`philo`)  
- `all` → Compila el programa  
- `clean` → Elimina archivos objeto  
- `fclean` → Elimina el ejecutable y archivos objeto  
- `re` → Limpia y recompila  

Para compilar el proyecto:  
```bash
make
```
## 🚀 Uso  
Ejecutar el programa con los siguientes argumentos:  
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
- `number_of_philosophers`: Número de filósofos en la simulación.  
- `time_to_die`: Tiempo en milisegundos antes de que un filósofo muera si no ha comido.  
- `time_to_eat`: Tiempo que un filósofo tarda en comer (en milisegundos).  
- `time_to_sleep`: Tiempo que un filósofo pasa durmiendo (en milisegundos).  
- `[number_of_times_each_philosopher_must_eat]` (opcional): Número de veces que cada filósofo debe comer antes de que el programa termine.  

### 📌 Ejemplo de ejecución  
```bash
./philo 5 800 200 200
```
Esto ejecutará la simulación con:

- 5 filósofos.
- `time_to_die = 800ms`.
- `time_to_eat = 200ms`.
- `time_to_sleep = 200ms`.

La simulación continuará hasta que un filósofo muera o hasta que todos los filósofos hayan comido `number_of_times_each_philosopher_must_eat` veces (si se especifica).

### 🏁 Ejemplo con límite de comidas
~~~bash
./philo 4 410 200 200 5
~~~
En este caso:
- 4 filósofos.
- `time_to_die = 410ms`.
- `time_to_eat = 200ms`.
- `time_to_sleep = 200ms`.
- Cada filósofo debe comer al menos 5 veces antes de que la simulación finalice.

## 🔧 Compilación y ejecución
Para compilar el proyecto, usa:
~~~bash
make
~~~
Esto generará el ejecutable `philo`. Para eliminar archivos compilados:
~~~bash
make clean
~~~
Para limpiar y recompilar desde cero:
~~~bash
make re
~~~

## ⚠️ Reglas del proyecto
- Cada filósofo es un **hilo independiente**.
- Cada tenedor es un **mutex** compartido entre dos filósofos.
- Se debe evitar **data races** mediante el uso adecuado de mutexes.
- **No se permite el uso de variables globales**.
- **El programa debe finalizar inmediatamente si un filósofo muere**.
- El **formato de logs** debe seguir el siguiente patrón:
  ~~~bash
  [timestamp] [philosopher_id] action
  ~~~
  Ejemplo:
  ~~~bash
  123 1 has taken a fork
  124 1 is eating
  ~~~

## 🛠 Errores comunes y debugging
Si el programa se comporta de manera inesperada, verifica:
- **Fugas de memoria** con `valgrind`:
  ~~~bash
  valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200
  ~~~
- **Condiciones de carrera** con `helgrind`:
  ~~~bash
  valgrind --tool=helgrind ./philo 5 800 200 200
  ~~~
- **Bloqueos (deadlocks)**: Asegúrate de que los mutexes siempre se desbloqueen correctamente.

## 📜 Eval
