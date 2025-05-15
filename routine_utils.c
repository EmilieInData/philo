/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:08:15 by esellier          #+#    #+#             */
/*   Updated: 2024/12/07 17:05:15 by esellier         ###   ########.fr       */
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

void	*check_die(void *arg)
{
	t_data			*d;
	t_thread		*philo;

	d = (t_data *)arg;
	philo = d->threads;
	usleep(5000);
	while (1)
	{
		if (pthread_mutex_lock(&d->status_mutex))
			return (write(2, "error: mutex not locked\n", 24), NULL);
		if (philo->status == END)
		{
			if (pthread_mutex_unlock(&d->status_mutex))
				write(2, "error: mutex not unlocked\n", 26);
			return (NULL);
		}
		if (calcul_time(philo->start_time) >= d->to_die && philo->status != EAT)
			return (do_die(d, philo));
		if (pthread_mutex_unlock(&d->status_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), NULL);
		philo = philo->next;
	}
}

void	*do_die(t_data *d, t_thread *philo)
{
	if (pthread_mutex_unlock(&d->status_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), NULL);
	if (pthread_mutex_lock(&d->general_mutex))
		return (write(2, "error: mutex not locked\n", 24), NULL);
	print_status(philo, DEAD, 0);
	if (pthread_mutex_unlock(&d->print_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), NULL);
	while (philo->life)
	{
		philo->life = 0;
		philo = philo->next;
	}
	if (pthread_mutex_unlock(&d->general_mutex))
		return (write(2, "error: mutex not unlocked\n", 26), NULL);
	return ((void *) 1);
}

void	print_eat(unsigned long time, t_thread *philo)
{
	printf("%ldms philo %d \033[38;2;255;138mhas taken a fork\033[0m\n",
		time, philo->position);
	printf("%ldms philo %d \033[38;2;255;138mhas taken a fork\033[0m\n",
		time, philo->position);
	printf("%ldms philo %d \033[38;2;255;184;154mis eating\033[0m\n",
		time, philo->position);
}

int	print_status(t_thread *philo, int i, unsigned long time)
{
	if (philo->life)
	{
		if (pthread_mutex_lock(&philo->data->print_mutex))
			return (write(2, "error: mutex not locked\n", 24), 1);
		time = calcul_time(philo->data->start_time);
		if (i == FORK)
			printf("%ldms philo %d \033[38;2;255;138mhas taken a fork\033[0m\n",
				time, philo->position);
		if (i == EAT)
			print_eat(time, philo);
		else if (i == SLEEP)
			printf("%ldms philo %d \033[38;2;220;20;60mis sleeping\033[0m\n",
				time, philo->position);
		else if (i == THINK)
			printf("%ldms philo %d \033[38;2;255;0;255mis thinking\033[0m\n",
				time, philo->position);
		else if (i == DEAD)
			return (printf("%ldms philo %d \033[38;2;255;0;0mdied\033[0m\n",
					time, philo->position), 0);
		if (pthread_mutex_unlock(&philo->data->print_mutex))
			return (write(2, "error: mutex not unlocked\n", 26), 1);
		return (0);
	}
	return (1);
}
