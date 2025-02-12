/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 10:54:56 by victor            #+#    #+#             */
/*   Updated: 2025/02/12 16:46:50 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ************************************************************************** */
/* Initializes mutexes and forks for the simulation.                          */
/* - Sets up the print and check mutexes.                                     */
/* - Initializes each fork's mutex.                                           */
/* ************************************************************************** */
static int	init_mutexes_and_forks(t_simulation *sim)
{
	int	i;

	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->check_mutex, NULL);
	i = 0;
	while (i < sim->num_philos)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/* Initializes philosopher structures for the simulation.                     */
/* - Assigns unique IDs, resets meal counters, and sets initial timestamps.   */
/* - Associates left and right forks with each philosopher.                   */
/* ************************************************************************** */
static void	init_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philos)
	{
		sim->philo[i].id = i + 1;
		sim->philo[i].meals_eaten = 0;
		sim->philo[i].last_meal_time = sim->start_time;
		sim->philo[i].left_fork = &sim->forks[i];
		sim->philo[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
		sim->philo[i].sim = sim;
		i++;
	}
}

/* ************************************************************************** */
/* Initializes the simulation parameters and allocates required resources.    */
/* - Validates command-line arguments.                                        */
/* - Sets simulation timing and meal parameters.                              */
/* - Allocates memory for forks and philosopher struct, then initializes them.*/
/* ************************************************************************** *
int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf(
				"./philo num time_die time_eat time_sleep [max_meals]\n"), 1);
	sim->num_philos = atoi(argv[1]);
	sim->time_to_die = atoi(argv[2]);
	sim->time_to_eat = atoi(argv[3]);
	sim->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		sim->max_meals = atoi(argv[5]);
	else
		sim->max_meals = -1;
	sim->finished_meals = 0;
	sim->dead = 0;
	sim->start_time = get_time();
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
	sim->philo = malloc(sizeof(t_philo) * sim->num_philos);
	if (!sim->forks || !sim->philo)
		return (printf("Memory allocation failed\n"), 1);
	if (init_mutexes_and_forks(sim))
		return (1);
	init_philosophers(sim);
	return (0);
}*/
int init_simulation(t_simulation *sim, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf("./philo num time_die time_eat time_sleep [max_meals]\n"), 1);
	sim->num_philos = atoi(argv[1]);
	sim->time_to_die = atoi(argv[2]);
	sim->time_to_eat = atoi(argv[3]);
	sim->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		sim->max_meals = atoi(argv[5]);
	else
		sim->max_meals = -1;
	sim->finished_meals = 0;
	sim->dead = 0;
	sim->start_time = get_time();
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
	sim->philo = malloc(sizeof(t_philo) * sim->num_philos);
	if (!sim->forks || !sim->philo)
		return (printf("Memory allocation failed\n"), 1);
	if (init_mutexes_and_forks(sim))
		return (1);
	init_philosophers(sim);
	sim->available_seats = sim->num_philos - 1;
	pthread_mutex_init(&sim->seat_mutex, NULL);
	return (0);
}


/* ************************************************************************** */
/* Cleans up and frees resources allocated for the simulation.                */
/* - Destroys each fork mutex and the general simulation mutexes.             */
/* - Frees memory allocated for forks and philosopher structures.             */
/* ************************************************************************** */
void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
		pthread_mutex_destroy(&sim->forks[i]);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->check_mutex);
	free(sim->forks);
	free(sim->philo);
}
