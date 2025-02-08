/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:10:27 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/08 20:43:03 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->sim->num_philos == 1)
	{
		pthread_mutex_lock(phil->left_fork);
		print_action(phil, "has taken a fork");
		precise_sleep(phil->sim->time_to_die);
		pthread_mutex_unlock(phil->left_fork);
		return (NULL);
	}
	if (phil->id % 2 == 0)
		precise_sleep(1);
	while (1)
	{
		pthread_mutex_lock(&phil->sim->check_mutex);
		if (phil->sim->dead || (phil->sim->max_meals
				> 0 && phil->meals_eaten >= phil->sim->max_meals))
		{
			pthread_mutex_unlock(&phil->sim->check_mutex);
			break ;
		}
		pthread_mutex_unlock(&phil->sim->check_mutex);
		take_forks(phil);
		print_action(phil, "is eating");
		pthread_mutex_lock(&phil->sim->check_mutex);
		phil->last_meal_time = get_time();
		phil->meals_eaten++;
		if (phil->sim->max_meals > 0 && phil->meals_eaten
			>= phil->sim->max_meals)
			phil->sim->finished_meals++;
		pthread_mutex_unlock(&phil->sim->check_mutex);
		precise_sleep(phil->sim->time_to_eat);
		put_forks(phil);
		print_action(phil, "is sleeping");
		precise_sleep(phil->sim->time_to_sleep);
		print_action(phil, "is thinking");
	}
	return (NULL);
}

/* ************************************************************************** */
void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->check_mutex);
		if (sim->max_meals > 0 && sim->finished_meals == sim->num_philos)
		{
			sim->dead = 1;
			printf("%ld All philos have eaten the number of times\n",
				get_time() - sim->start_time);
			pthread_mutex_unlock(&sim->check_mutex);
			break ;
		}
		i = -1;
		while (++i < sim->num_philos)
		{
			if (get_time() - sim->philo[i].last_meal_time > sim->time_to_die)
			{
				sim->dead = 1;
				printf("%ld %d died\n", get_time()
					- sim->start_time, sim->philo[i].id);
				pthread_mutex_unlock(&sim->check_mutex);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&sim->check_mutex);
		usleep(1000);
	}
	return (NULL);
}

int	init_simulation(t_simulation *sim, int argc, char **argv)
{
	int	i;

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
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->check_mutex, NULL);
	i = -1;
	while (++i < sim->num_philos)
		pthread_mutex_init(&sim->forks[i], NULL);
	i = -1;
	while (++i < sim->num_philos)
	{
		sim->philo[i].id = i + 1;
		sim->philo[i].meals_eaten = 0;
		sim->philo[i].last_meal_time = sim->start_time;
		sim->philo[i].left_fork = &sim->forks[i];
		sim->philo[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
		sim->philo[i].sim = sim;
	}
	return (0);
}

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

/* ************************************************************************** */
int	main(int argc, char **argv)
{
	t_simulation	sim;
	pthread_t		monitor;
	int				i;

	if (init_simulation(&sim, argc, argv))
		return (1);
	i = -1;
	while (++i < sim.num_philos)
		pthread_create(&sim.philo[i].thread, NULL,
			philo_routine, &sim.philo[i]);
	pthread_create(&monitor, NULL, monitor_routine, &sim);
	i = -1;
	while (++i < sim.num_philos)
		pthread_join(sim.philo[i].thread, NULL);
	pthread_join(monitor, NULL);
	cleanup_simulation(&sim);
	return (0);
}
