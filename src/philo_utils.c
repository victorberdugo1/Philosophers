/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 20:13:26 by victor            #+#    #+#             */
/*   Updated: 2025/02/16 11:30:14 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ************************************************************************** */
/* Returns the current time in milliseconds.                                */
/* - Uses gettimeofday to compute the time elapsed in ms.                   */
/* ************************************************************************** */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* ************************************************************************** */
/* Sleeps for a specified number of milliseconds with high precision.       */
/* - Continuously checks the elapsed time and uses usleep to yield control.   */
/* ************************************************************************** */
void	precise_sleep(int milliseconds)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(100);
}

/* ************************************************************************** */
/* Prints a philosopher's action with a timestamp.                          */
/* - Locks the print mutex to ensure exclusive access to stdout.            */
/* - Only prints if the simulation is not flagged as dead.                  */
/* ************************************************************************** */
void	print_action(t_philo *phil, char *message)
{
	pthread_mutex_lock(&phil->sim->check_mutex);
	if (!phil->sim->dead)
	{
		ft_printf("%d %d %s\n", get_time() - phil->sim->start_time,
			phil->id, message);
	}
	pthread_mutex_unlock(&phil->sim->check_mutex);
}

/* ************************************************************************** */
/* Handles the action of taking forks by a philosopher.                     */
/* - For even-numbered philosophers, picks up the right fork first.         */
/* - For odd-numbered philosophers, picks up the left fork first.           */
/* - Prints each fork-taking action accordingly.                            */
/* ************************************************************************** */
void	take_forks(t_philo *phil)
{
	if (phil->id % 2 == 0)
	{
		pthread_mutex_lock(phil->right_fork);
		print_action(phil, "has taken a fork");
		pthread_mutex_lock(phil->left_fork);
		print_action(phil, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(phil->left_fork);
		print_action(phil, "has taken a fork");
		pthread_mutex_lock(phil->right_fork);
		print_action(phil, "has taken a fork");
	}
}

/* ************************************************************************** */
/* Releases the forks held by a philosopher.                                */
/* - Unlocks both the left and right fork mutexes.                          */
/* ************************************************************************** */
void	put_forks(t_philo *phil)
{
	pthread_mutex_unlock(phil->left_fork);
	pthread_mutex_unlock(phil->right_fork);
}
