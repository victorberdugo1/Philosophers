/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:16:41 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/07 13:30:03 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, free
#include <string.h>     // memset
#include <unistd.h>     // write, usleep
#include <sys/time.h>   // gettimeofday
#include <pthread.h>    // pthread functions
#include <stdbool.h>

// Estructura para cada filósofo
typedef struct s_philosopher {
    int             id;
    int             meals_eaten;
    long            last_meal;
    pthread_t       thread;
}               t_philosopher;

// Estructura de la simulación
typedef struct s_simulation {
    int             num_philos;       // Número de filósofos
    int             time_to_die;      // Tiempo antes de morir sin comer (ms)
    int             time_to_eat;      // Tiempo que se tarda en comer (ms)
    int             time_to_sleep;    // Tiempo que se duerme (ms)
    int             meals_required;   // Número de veces que deben comer (opcional, -1 si no se especifica)
    long            start_time;       // Momento en que inicia la simulación (en ms)
    t_philosopher   *philosophers;    // Array de filósofos
    pthread_mutex_t *forks;           // Array de mutex, uno por cada tenedor
    pthread_mutex_t print_mutex;      // Mutex para sincronizar la impresión
    bool            simulation_over;  // Flag para finalizar la simulación
}               t_simulation;

// Prototipos de funciones
int init_simulation(t_simulation *sim, int argc, char **argv);
int start_simulation(t_simulation *sim);
void monitor_simulation(t_simulation *sim);
void cleanup_simulation(t_simulation *sim);

#endif
