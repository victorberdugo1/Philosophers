/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:10:27 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/07 13:29:19 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Devuelve el tiempo actual en milisegundos
long current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Rutina que ejecuta cada filósofo (implementación simplificada)
void *philosopher_routine(void *arg)
{
    t_philosopher *phil = (t_philosopher *)arg;
    while (1)
    {
        // Estado: pensando
        pthread_mutex_lock(&((t_simulation *)NULL)->print_mutex); // Solo para ilustrar la sincronización
        printf("Philosopher %d is thinking\n", phil->id);
        pthread_mutex_unlock(&((t_simulation *)NULL)->print_mutex);
        usleep(100000);

        // Estado: comiendo
        pthread_mutex_lock(&((t_simulation *)NULL)->print_mutex);
        printf("Philosopher %d is eating\n", phil->id);
        pthread_mutex_unlock(&((t_simulation *)NULL)->print_mutex);
        phil->last_meal = current_time();
        usleep(100000);

        // Estado: durmiendo
        pthread_mutex_lock(&((t_simulation *)NULL)->print_mutex);
        printf("Philosopher %d is sleeping\n", phil->id);
        pthread_mutex_unlock(&((t_simulation *)NULL)->print_mutex);
        usleep(100000);
    }
    return NULL;
}

// Inicializa la simulación: asigna parámetros y recursos
int init_simulation(t_simulation *sim, int argc, char **argv)
{
    int i;

    sim->num_philos = atoi(argv[1]);
    sim->time_to_die = atoi(argv[2]);
    sim->time_to_eat = atoi(argv[3]);
    sim->time_to_sleep = atoi(argv[4]);
    if (argc == 6)
        sim->meals_required = atoi(argv[5]);
    else
        sim->meals_required = -1; // No se especificó
    sim->simulation_over = false;
    sim->start_time = current_time();

    sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philos);
    if (!sim->philosophers)
        return (1);

    sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
    if (!sim->forks)
        return (1);

    for (i = 0; i < sim->num_philos; i++)
    {
        sim->philosophers[i].id = i + 1;
        sim->philosophers[i].meals_eaten = 0;
        sim->philosophers[i].last_meal = sim->start_time;
        if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
            return (1);
    }
    pthread_mutex_init(&sim->print_mutex, NULL);
    return (0);
}

// Crea los hilos de los filósofos y los inicia
int start_simulation(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philos; i++)
    {
        if (pthread_create(&sim->philosophers[i].thread, NULL,
            philosopher_routine, &sim->philosophers[i]) != 0)
            return (1);
    }
    return (0);
}

// Función de monitoreo simplificada: espera unos segundos y finaliza la simulación
void monitor_simulation(t_simulation *sim)
{
    // En una implementación real, aquí se debería monitorear el estado de cada filósofo.
    sleep(3);
    sim->simulation_over = true;
    printf("Simulation ended.\n");
}

// Libera los recursos asignados a la simulación
void cleanup_simulation(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philos; i++)
    {
        pthread_detach(sim->philosophers[i].thread);
        pthread_mutex_destroy(&sim->forks[i]);
    }
    pthread_mutex_destroy(&sim->print_mutex);
    free(sim->forks);
    free(sim->philosophers);
}

int main(int argc, char **argv)
{
    t_simulation sim;

    if (argc != 5 && argc != 6)
    {
        fprintf(stderr,
            "Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
            argv[0]);
        return (1);
    }
    if (init_simulation(&sim, argc, argv))
    {
        fprintf(stderr, "Error initializing simulation\n");
        return (1);
    }
    if (start_simulation(&sim))
    {
        fprintf(stderr, "Error starting simulation\n");
        cleanup_simulation(&sim);
        return (1);
    }
    monitor_simulation(&sim);
    cleanup_simulation(&sim);
    return (0);
}
