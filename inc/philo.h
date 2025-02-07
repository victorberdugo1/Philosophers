/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:16:41 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/07 14:20:01 by vberdugo         ###   ########.fr       */
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


typedef struct s_philosopher t_philosopher;

// Estructura de la simulación
typedef struct s_simulation
{
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int meals_required;
    bool simulation_over;
    long start_time;
    t_philosopher *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    int meals_completed; // Contador de filósofos que han completado sus comidas
} t_simulation;

typedef struct s_philosopher {
    int             id;
    pthread_t       thread;
    int             meals_eaten;
    long            last_meal;
    pthread_mutex_t meal_mutex; // Agregar esta línea
    struct s_simulation *sim;
} t_philosopher;

// Prototipos de funciones
void *philosopher_routine(void *arg);



#endif
