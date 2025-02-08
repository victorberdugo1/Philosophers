/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 20:13:26 by victor            #+#    #+#             */
/*   Updated: 2025/02/08 20:14:04 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(int milliseconds)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
}

void	print_action(t_philo *phil, char *message)
{
	pthread_mutex_lock(&phil->sim->print_mutex);
	if (!phil->sim->dead)
		printf("%ld %d %s\n", get_time()
			- phil->sim->start_time, phil->id, message);
	pthread_mutex_unlock(&phil->sim->print_mutex);
}

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

void	put_forks(t_philo *phil)
{
	pthread_mutex_unlock(phil->left_fork);
	pthread_mutex_unlock(phil->right_fork);
}
