/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberdugo <vberdugo@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:10:27 by vberdugo          #+#    #+#             */
/*   Updated: 2025/02/12 17:44:46 by vberdugo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ************************************************************************** */
/* Handles the case when there is only one philosopher.                       */
/* - Locks the only available fork.                                           */
/* - Prints the action of taking the fork.                                    */
/* - Sleeps for the entire time to die (simulating inevitable death).         */
/* - Unlocks the fork and returns NULL.                                       */
/* ************************************************************************** */
void	*handle_single_philo(t_philo *phil)
{
	pthread_mutex_lock(phil->left_fork);
	print_action(phil, "has taken a fork");
	precise_sleep(phil->sim->time_to_die);
	pthread_mutex_unlock(phil->left_fork);
	return (NULL);
}

/* ************************************************************************** */
/* Executes one complete cycle of a philosopher's actions.                    */
/* - Takes both forks and prints the eating action.                           */
/* - Updates the last meal time and increments the meal counter.              */
/* - Sleeps for the eating duration, then releases forks, sleeps, and thinks. */
/* ************************************************************************** */
static void philo_cycle(t_philo *phil)
{
	pthread_mutex_lock(&phil->sim->seat_mutex);
	while (phil->sim->available_seats <= 0)
	{
		pthread_mutex_unlock(&phil->sim->seat_mutex);
		usleep(100);
		pthread_mutex_lock(&phil->sim->seat_mutex);
	}
	phil->sim->available_seats--;
	pthread_mutex_unlock(&phil->sim->seat_mutex);

	take_forks(phil);
	print_action(phil, "is eating");

	pthread_mutex_lock(&phil->sim->check_mutex);
	phil->last_meal_time = get_time();
	phil->meals_eaten++;
	if (phil->sim->max_meals > 0 && phil->meals_eaten >= phil->sim->max_meals)
		phil->sim->finished_meals++;
	pthread_mutex_unlock(&phil->sim->check_mutex);

	precise_sleep(phil->sim->time_to_eat);
	put_forks(phil);

	pthread_mutex_lock(&phil->sim->seat_mutex);
	phil->sim->available_seats++;
	pthread_mutex_unlock(&phil->sim->seat_mutex);

	print_action(phil, "is sleeping");
	precise_sleep(phil->sim->time_to_sleep);
	print_action(phil, "is thinking");
}

/* ************************************************************************** */
/* Main routine executed by each philosopher thread.                          */
/* - For a single philosopher, calls handle_single_philo.                     */
/* - Delays even-numbered philosophers slightly to help avoid deadlocks.      */
/* - Continuously performs action cycles until termination conditions occur.  */
/* ************************************************************************** */
void	*philo_routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->sim->num_philos == 1)
		return (handle_single_philo(phil), NULL);
	if (phil->id % 2 == 0)
		precise_sleep(1);
	while (1)
	{
		pthread_mutex_lock(&phil->sim->check_mutex);
		if (phil->sim->dead || (phil->sim->max_meals > 0 && phil->meals_eaten >= phil->sim->max_meals))
		{
			pthread_mutex_unlock(&phil->sim->check_mutex);
			break ;
		}
		pthread_mutex_unlock(&phil->sim->check_mutex);
		philo_cycle(phil);
	}
	return (NULL);
}


/* ************************************************************************** */
/* Monitors simulation conditions to detect termination.                      */
/* - Checks if all philosophers have eaten the required number of meals.      */
/* - Iterates through each philosopher to detect if any have starved.         */
/* - Sets the simulation dead flag and prints messages when termination.      */
/* ************************************************************************** */
void *monitor_routine(void *arg)
{
	t_simulation *sim;
	int i;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->check_mutex);
		if (sim->max_meals > 0 && sim->finished_meals == sim->num_philos)
		{
			sim->dead = 1;
			pthread_mutex_lock(&sim->print_mutex);
			ft_printf("%d All philos fed.\n", get_time() - sim->start_time);
			pthread_mutex_unlock(&sim->print_mutex);
			pthread_mutex_unlock(&sim->check_mutex);
			break;
		}
		i = -1;
		while (++i < sim->num_philos)
		{
			if (get_time() - sim->philo[i].last_meal_time > sim->time_to_die)
			{
				sim->dead = 1;
				pthread_mutex_lock(&sim->print_mutex);
				ft_printf("%d %d died\n", get_time() - sim->start_time, sim->philo[i].id);
				pthread_mutex_unlock(&sim->print_mutex);
				pthread_mutex_unlock(&sim->check_mutex);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&sim->check_mutex);
		usleep(1000);
	}
	return (NULL);
}

/* ************************************************************************** */
/* Entry point of the simulation program.                                     */
/* - Initializes the simulation environment and creates philosopher threads.  */
/* - Starts the monitor thread and waits for all threads to finish.           */
/* - Cleans up allocated resources before exiting.                            */
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
