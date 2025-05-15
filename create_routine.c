/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:45:32 by esellier          #+#    #+#             */
/*   Updated: 2024/12/07 17:04:18 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	do_eat(t_thread *philo)
{
	if (pthread_mutex_lock(&philo->data->general_mutex))
		return (write(2, "error: mutex not locked\n", 24), 1);
	if (philo->life)
	{
		if (pthread_mutex_unlock(&philo->data->general_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		if (pthread_mutex_lock(&philo->fork))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->fork_status = 1;
		if (pthread_mutex_lock(&philo->next->fork))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->next->fork_status = 1;
		if (pthread_mutex_lock(&philo->data->status_mutex))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->status = EAT;
		if (pthread_mutex_unlock(&philo->data->status_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		if (pthread_mutex_lock(&philo->data->general_mutex))
			return (write(2, "error: mutex not locked\n", 24), 1);
		return (do_eat_end(philo));
	}
	if (pthread_mutex_unlock(&philo->data->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	return (1);
}

int	do_eat_end(t_thread *philo)
{
	if (print_status(philo, EAT, 0))
	{
		if (pthread_mutex_unlock(&philo->data->general_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		if (pthread_mutex_unlock(&philo->fork)
			|| pthread_mutex_unlock(&philo->next->fork))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		return (1);
	}
	if (pthread_mutex_unlock(&philo->data->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	gettimeofday(&philo->start_time, NULL);
	usleep(philo->data->to_eat * 1000);
	philo->fork_status = 0;
	if (pthread_mutex_unlock(&philo->fork))
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	philo->next->fork_status = 0;
	if (pthread_mutex_unlock(&philo->next->fork) != 0)
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	return (0);
}

int	do_sleep(t_thread *philo)
{
	if (pthread_mutex_lock(&philo->data->general_mutex))
		return (write(2, "error: mutex not locked\n", 24), 1);
	if (philo->life)
	{
		if (pthread_mutex_unlock(&philo->data->general_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		if (pthread_mutex_lock(&philo->data->status_mutex))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->status = SLEEP;
		if (pthread_mutex_unlock(&philo->data->status_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		print_status(philo, SLEEP, 0);
		usleep(philo->data->to_sleep * 1000);
		return (0);
	}
	if (pthread_mutex_unlock(&philo->data->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	return (1);
}

int	do_think(t_thread *philo)
{
	if (pthread_mutex_lock(&philo->data->general_mutex))
		return (write(2, "error: mutex not locked\n", 24), 1);
	if (philo->life)
	{
		if (pthread_mutex_unlock(&philo->data->general_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		if (pthread_mutex_lock(&philo->data->status_mutex))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->status = THINK;
		if (pthread_mutex_unlock(&philo->data->status_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		print_status(philo, THINK, 0);
		return (0);
	}
	if (pthread_mutex_unlock(&philo->data->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), 1);
	return (1);
}

void	*do_routine(void *arg)
{
	t_thread	*philo;

	philo = (t_thread *)arg;
	gettimeofday(&philo->start_time, NULL);
	pthread_mutex_lock(&philo->data->general_mutex);
	pthread_mutex_unlock(&philo->data->general_mutex);
	if (philo->position % 2 != 0)
		usleep(philo->data->to_eat * 1000 / 2);
	while (philo->nb_eat != 0)
	{
		if (do_eat(philo) == 1)
			break ;
		if (do_sleep(philo) == 1)
			break ;
		if (do_think(philo) == 1)
			break ;
		philo->nb_eat--;
	}
	if (pthread_mutex_lock(&philo->data->status_mutex))
		return (write(2, "error: mutex not locked\n", 24), NULL);
	philo->status = END;
	if (pthread_mutex_unlock(&philo->data->status_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), NULL);
	return (NULL);
}
