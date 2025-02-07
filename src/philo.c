/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:10:27 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/07 14:16:43 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 ** Devuelve el tiempo actual en milisegundos.
 */
long get_time_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*
 ** Imprime una acción con mutex para evitar colisiones de impresión.
 */
void print_action(t_simulation *sim, int id, const char *action)
{
    long timestamp;
    pthread_mutex_lock(&sim->print_mutex);
    timestamp = get_time_in_ms() - sim->start_time;
    printf("%ld %d %s\n", timestamp, id, action);
    pthread_mutex_unlock(&sim->print_mutex);
}

/*
 ** Inicializa la simulación.
 */
int init_simulation(t_simulation *sim, int argc, char **argv)
{
    int i;
    if (argc < 5 || argc > 6)
        return (fprintf(stderr, "Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_required]\n", argv[0]), 1);

    sim->num_philos = atoi(argv[1]);
    sim->time_to_die = atoi(argv[2]);
    sim->time_to_eat = atoi(argv[3]);
    sim->time_to_sleep = atoi(argv[4]);
    sim->meals_required = (argc == 6) ? atoi(argv[5]) : -1;
    sim->simulation_over = false;
    sim->start_time = get_time_in_ms();
    sim->meals_completed = 0; // Inicializa el contador de comidas completadas

    sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philos);
    sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
    if (!sim->philosophers || !sim->forks)
        return (1);

    pthread_mutex_init(&sim->print_mutex, NULL);
    for (i = 0; i < sim->num_philos; i++)
    {
        pthread_mutex_init(&sim->forks[i], NULL);
        pthread_mutex_init(&sim->philosophers[i].meal_mutex, NULL);
        sim->philosophers[i] = (t_philosopher){ .id = i + 1, .last_meal = sim->start_time, .meals_eaten = 0, .sim = sim };
    }
    return (0);
}

/*
 ** Función para iniciar la simulación (crear hilos).
 */
int start_simulation(t_simulation *sim)
{
    int i;

    for (i = 0; i < sim->num_philos; i++)
    {
        if (pthread_create(&sim->philosophers[i].thread, NULL, philosopher_routine, &sim->philosophers[i]) != 0)
            return (1);
    }
    return (0);
}

/*
 ** Rutina de cada filósofo.
 */
void *philosopher_routine(void *arg)
{
    t_philosopher *phil = (t_philosopher *)arg;
    t_simulation *sim = phil->sim;
    int left = phil->id - 1;
    int right = (phil->id) % sim->num_philos;

    if (sim->num_philos == 1)
    {
        pthread_mutex_lock(&sim->forks[left]);
        print_action(sim, phil->id, "has taken a fork");
        while (!sim->simulation_over) usleep(1000);
        pthread_mutex_unlock(&sim->forks[left]);
        return (NULL);
    }

    while (!sim->simulation_over)
    {
        if (phil->id % 2 == 0)
        {
            pthread_mutex_lock(&sim->forks[right]);
            print_action(sim, phil->id, "has taken a fork");
            pthread_mutex_lock(&sim->forks[left]);
        }
        else
        {
            pthread_mutex_lock(&sim->forks[left]);
            print_action(sim, phil->id, "has taken a fork");
            pthread_mutex_lock(&sim->forks[right]);
        }
        print_action(sim, phil->id, "is eating");
        pthread_mutex_lock(&phil->meal_mutex);
        phil->last_meal = get_time_in_ms();
        pthread_mutex_unlock(&phil->meal_mutex);
        usleep(sim->time_to_eat * 1000);
        phil->meals_eaten++;
        pthread_mutex_unlock(&sim->forks[left]);
        pthread_mutex_unlock(&sim->forks[right]);

        // Verifica si el filósofo ha comido lo suficiente
        if (sim->meals_required != -1 && phil->meals_eaten >= sim->meals_required)
        {
            pthread_mutex_lock(&sim->print_mutex);
            sim->meals_completed++;
            pthread_mutex_unlock(&sim->print_mutex);
        }

        print_action(sim, phil->id, "is sleeping");
        usleep(sim->time_to_sleep * 1000);
        print_action(sim, phil->id, "is thinking");
    }
    return (NULL);
}
/*
 ** Monitoreo de la simulación.
 */
void monitor_simulation(t_simulation *sim)
{
    int i;
    while (!sim->simulation_over)
    {
        for (i = 0; i < sim->num_philos; i++)
        {
            pthread_mutex_lock(&sim->philosophers[i].meal_mutex);
            if (get_time_in_ms() - sim->philosophers[i].last_meal >= sim->time_to_die)
            {
                pthread_mutex_unlock(&sim->philosophers[i].meal_mutex);
                printf("%ld %d died\n", get_time_in_ms() - sim->start_time, sim->philosophers[i].id);
                sim->simulation_over = true;
                return;
            }
            pthread_mutex_unlock(&sim->philosophers[i].meal_mutex);
        }

        // Verifica si todos los filósofos han comido lo suficiente
        if (sim->meals_required != -1 && sim->meals_completed >= sim->num_philos)
        {
            printf("%ld All philosophers have eaten %d times\n", get_time_in_ms() - sim->start_time, sim->meals_required);
            sim->simulation_over = true;
            return;
        }

        usleep(500);
    }
}
/*
 ** Limpieza de la simulación.
 */
void cleanup_simulation(t_simulation *sim)
{
    int i;
    for (i = 0; i < sim->num_philos; i++)
    {
        pthread_join(sim->philosophers[i].thread, NULL);
        pthread_mutex_destroy(&sim->forks[i]);
        pthread_mutex_destroy(&sim->philosophers[i].meal_mutex);
    }
    pthread_mutex_destroy(&sim->print_mutex);
    free(sim->forks);
    free(sim->philosophers);
}

/*
 ** Función principal.
 */
int main(int argc, char **argv)
{
    t_simulation sim;
    if (argc != 5 && argc != 6)
        return (fprintf(stderr, "Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_required]\n", argv[0]), 1);
    if (init_simulation(&sim, argc, argv))
        return (fprintf(stderr, "Error initializing simulation\n"), 1);
    if (start_simulation(&sim))
        return (fprintf(stderr, "Error starting simulation\n"), cleanup_simulation(&sim), 1);
    monitor_simulation(&sim);
    cleanup_simulation(&sim);
    return (0);
}

