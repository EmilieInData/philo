/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:08:15 by esellier          #+#    #+#             */
/*   Updated: 2024/12/04 09:49:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	calcul_time(struct timeval start)
{
	unsigned long		time;
	struct timeval		tmp;

	gettimeofday(&tmp, NULL);
	time = (tmp.tv_sec - start.tv_sec) * 1000
		+ (tmp.tv_usec - start.tv_usec) * 0.001;
	return (time);
}

void	do_mutex(t_data *data)
{
	unsigned int	count;
	t_thread		*current;

	count = 0;
	current = data->threads;
	while (current && count < data->philo)
	{
		if (pthread_mutex_init(&current->fork, NULL) != 0)
			error_msg("error: mutex not created\n", data, current, 0);
		current = current->next;
		count++;
	}
	return ;
}

void	*check_die(void *arg)
{
	unsigned long	time;
	t_data			*d;
	t_thread		*philo;

	d = (t_data *)arg;
	philo = d->threads;
	while (1)
	{
		if (philo->status == END)
			return (NULL);
		//if (philo->start_time.tv_sec == 0)
		//	philo = philo->next;
		time = calcul_time(philo->start_time);
		if (time >= d->to_die && philo->status != EAT) //&& philo->status != END)
		{
			pthread_mutex_lock(&d->general_mutex);
			//time = calcul_time(d->start_time);
			//pthread_mutex_lock(&d->print_mutex);
			//printf("%ldms philo %d \033[38;2;255;0;0mdied\033[0m\n",
			//	time, philo->position);
			print_status(philo, DEAD);
			while (philo->life)
			{
				philo->life = 0;
				philo = philo->next;
			}
			pthread_mutex_unlock(&d->general_mutex);
			return ((void *) 1);
		}
		philo = philo->next;
	}
	return (NULL);
}

int	print_status(t_thread *philo, int i)
{
	unsigned long	time;
	
	if (philo->life)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		time = calcul_time(philo->data->start_time);
		if (i == EAT)
		{
			printf("%ldms philo %d \033[38;2;255;138mhas taken a fork\033[0m\n",
				time, philo->position);
			printf("%ldms philo %d \033[38;2;255;138mhas taken a fork\033[0m\n",
				time, philo->position);
			printf("%ldms philo %d \033[38;2;255;184;154mis eating\033[0m\n",
				time, philo->position);
		}				
		else if (i == SLEEP)
			printf("%ldms philo %d \033[38;2;220;20;60mis sleeping\033[0m\n",
				time, philo->position);
		else if (i == THINK)
			printf("%ldms philo %d \033[38;2;255;0;255mis thinking\033[0m\n",
				time, philo->position);
		else if (i == DEAD)
			return (printf("%ldms philo %d \033[38;2;255;0;0mdied\033[0m\n",
				time, philo->position), 0);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (0);
	}
	return (1);
}

void	*destroy_mutex(t_thread *current, unsigned int philo)
{
	unsigned int	count;

	count = 0;
	if (pthread_mutex_destroy(&current->data->print_mutex)
		|| pthread_mutex_destroy(&current->data->general_mutex))
	{
		write(2, "error: mutex destroy failed\n", 28);
		return (NULL);
	}
	while (current && count < philo)
	{
		if (current && current->fork_status)
		{
			if (pthread_mutex_unlock(&current->fork))
				return (write(2, "error: mutex not unlocked\n", 26), NULL);
		}
		if (pthread_mutex_destroy(&current->fork))
		{
			write(2, "error: mutex destroy failed\n", 28);
			return (NULL);
		}
		current = current->next;
		count++;
	}
	return (NULL);
}
