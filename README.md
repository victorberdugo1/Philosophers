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
# Mandatory Part

## Error Handling

This project is to be coded in C, following the Norm.  
Any crash, undefined behavior, memory leak, or norm error means 0 to the project.  
On some slow hardware, the project might not work properly. If some tests don't work on your machine, try to discuss it honestly before counting it as false.

- [ ] Yes
- [ ] No

## Global Variables

Check if there is any global variable which is used to manage the shared resources among the philosophers.  
If you find such a nasty thing, the evaluation stops here. You can go on and check the code, but do not grade the exercises.

- [ ] Yes
- [ ] No

## Philo Code

- Ensure the code of philo complies with the following requirements and ask for explanations.
  - Check there is one thread per philosopher.
  - Check there is only one fork per philosopher.
  - Check if there is a mutex per fork and that it's used to check the fork value and/or change it.
  - Check the outputs are never mixed up.
  - Check how the death of a philosopher is verified and if there is a mutex to prevent a philosopher from dying and starting eating at the same time.

- [ ] Yes
- [ ] No

## Philo Testing

Do not test with more than 200 philosophers.  
Do not test with `time_to_die` or `time_to_eat` or `time_to_sleep` set to values lower than 60 ms.

- Test 1 800 200 200. The philosopher should not eat and should die.
- Test 5 800 200 200. No philosopher should die.
- Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
- Test 4 410 200 200. No philosopher should die.
- Test 4 310 200 100. One philosopher should die.
- Test with 2 philosophers and check the different times: a death delayed by more than 10 ms is unacceptable.
- Test with any values of your choice to verify all the requirements. Ensure philosophers die at the right time, that they don’t steal forks, and so forth.

- [ ] Yes
- [ ] No

# Bonus Part

## Philo Bonus Code

Ensure the code of philo_bonus complies with the following requirements and ask for explanations.

- Check that there is one process per philosopher and that the main process is not a philosopher.
- Check that there are no orphan processes at the end of the execution of this program.
- Check if there is a single semaphore that represents the number of forks.
- Check the output is protected against multiple access. To avoid a scrambled display.
- Check how the death of a philosopher is verified and if there is a semaphore to prevent a philosopher from dying and starting eating at the same time.

- [ ] Yes
- [ ] No
