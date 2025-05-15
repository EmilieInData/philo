/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:42:29 by esellier          #+#    #+#             */
/*   Updated: 2024/12/07 17:04:44 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

ssize_t	do_threads(t_data *d)
{
	unsigned int	count;
	t_thread		*current;
	void			*rtrn;

	count = 0;
	current = d->threads;
	if (pthread_mutex_lock(&d->general_mutex))
		return (write(2, "error: mutex not locked\n", 24));
	while (current && count < d->philo)
	{
		if (pthread_create(&current->thread_id, NULL, do_routine,
				(void *)current))
			return (write(2, "error: thread not created\n", 26));
		current = current->next;
		count++;
	}
	if (pthread_mutex_unlock(&d->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26));
	if (pthread_create(&d->thread_die, NULL, check_die, (void *)d))
		return (write(2, "error: thread not created\n", 26));
	if (pthread_join(d->thread_die, &rtrn))
		return (write(2, "error: thread join failed\n", 26));
	return (0);
}

void	do_mutex(t_data *data)
{
	unsigned int	count;
	t_thread		*current;

	count = 0;
	current = data->threads;
	if (pthread_mutex_init(&data->status_mutex, NULL))
		error_msg("error: mutex not created\n", data, current, 1);
	if (pthread_mutex_init(&data->general_mutex, NULL))
		error_msg("error: mutex not created\n", data, current, 1);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		error_msg("error: mutex not created\n", data, current, 1);
	while (current && count < data->philo)
	{
		if (pthread_mutex_init(&current->fork, NULL))
		{
			if (pthread_mutex_destroy(&data->general_mutex)
				|| pthread_mutex_destroy(&data->print_mutex)
				|| pthread_mutex_destroy(&data->status_mutex))
				error_msg("error: mutex destroy failed\n", data, current, 0);
			error_msg("error: mutex not created\n", data, current, 0);
		}
		current = current->next;
		count++;
	}
}

void	*destroy_mutex(t_thread *current, unsigned int philo)
{
	unsigned int	count;

	count = 0;
	if (pthread_mutex_destroy(&current->data->print_mutex)
		|| pthread_mutex_destroy(&current->data->general_mutex)
		|| pthread_mutex_destroy(&current->data->status_mutex))
	{
		write(2, "error: mutex destroy failed\n", 28);
		return (NULL);
	}
	while (current && count < philo)
	{
		if (pthread_mutex_destroy(&current->fork))
			return (write(2, "error: mutex destroy failed\n", 28), NULL);
		current = current->next;
		count++;
	}
	return (NULL);
}

void	do_philo(t_thread *philo, t_data *data)
{
	if (data->nb_eat != 0)
	{
		if (pthread_mutex_init(&data->print_mutex, NULL))
			error_msg("error: mutex not created\n", data, philo, 0);
		gettimeofday(&philo->start_time, NULL);
		print_status(philo, FORK, 0);
		usleep(philo->data->to_die * 1000);
		print_status(philo, DEAD, 0);
		if (pthread_mutex_unlock(&data->print_mutex))
			error_msg("error: mutex not unlocked\n", data, philo, 0);
		if (pthread_mutex_destroy(&data->print_mutex))
			error_msg("error: mutex destroy failed\n", data, philo, 0);
	}
	free_philo(philo->data);
	exit (0);
}
