/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:45:32 by esellier          #+#    #+#             */
/*   Updated: 2024/12/03 16:58:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	do_eat(t_thread *philo)
{
	if (philo->life)
	{
		if (pthread_mutex_lock(&philo->fork))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->fork_status = 1;
		if (pthread_mutex_lock(&philo->next->fork))
			return (write(2, "error: mutex not locked\n", 24), 1);
		philo->next->fork_status = 1;
		philo->status = EAT;
		if (print_status(philo, EAT))
			return (1);
		gettimeofday(&philo->start_time, NULL);
		usleep(philo->data->to_eat * 1000);
		if (pthread_mutex_unlock(&philo->fork) != 0)
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		philo->fork_status = 0;
		if (pthread_mutex_unlock(&philo->next->fork) != 0)
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		philo->next->fork_status = 0;
		philo->status = NEUTRAL;
		return (0);
	}
	return (1);
}

int	do_sleep(t_thread *philo)
{
	if (philo->life)
	{
		philo->status = SLEEP;
		print_status(philo, SLEEP);
		usleep(philo->data->to_sleep * 1000);
		philo->status = NEUTRAL;
		return (0);
	}
	return (1);
}

int	do_think(t_thread *philo)
{
	if (philo->life)
	{
		philo->status = THINK;
		print_status(philo, THINK);
		philo->status = NEUTRAL;
		return (0);
	}
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
	philo->status = END;
	return (NULL);
}

void	do_threads(t_data *d)
{
	unsigned int	count;
	t_thread		*current;
	void			*rtrn;
	//int				tmp;

	count = 0;
	current = d->threads;
	pthread_mutex_lock(&d->general_mutex);
	while (current && count < d->philo)
	{
		if (pthread_create(&current->thread_id, NULL, do_routine,
				(void *)current))
			error_msg("error: thread not created\n", d, d->threads, 1);
		current = current->next;
		count++;
	}
	pthread_mutex_unlock(&d->general_mutex);
	if (pthread_create(&d->thread_die, NULL, check_die, (void *)d))
		error_msg("error: thread not created\n", d, d->threads, 1);
	if (pthread_join(d->thread_die, &rtrn))
		write(2, "error: thread join failed\n", 26);
	if (rtrn)
		pthread_mutex_unlock(&d->print_mutex);	
	//pthread_mutex_unlock(&d->general_mutex);
	//pthread_mutex_unlock(&d->print_mutex);
}
//verifier la facon de checker si un philo est mort
//checker avec un seul philo
//checker avec  0 repas, return (0) direct 
//checker si necessaire de checker la fouchette 2 si on prend la 1 et que la 2 pas dispo;

	//printf("TIME = %ld\n", data->start_time.tv_usec);
	//printf("Adresse de start_time : %p\n", (void *)&data->start_time);
	//printf("Adresse de data : %p\n", (void *)data);

//			philo->start_time.tv_sec = philo->start_time.tv_sec - philo->data->start_time.tv_sec;
//		philo->start_time.tv_usec = philo->start_time.tv_usec - philo->data->start_time.tv_usec;