/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:27:03 by esellier          #+#    #+#             */
/*   Updated: 2024/12/06 19:52:49 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_args(char **array)
{
	int	i;
	int	j;

	i = 1;
	while (array[i])
	{
		j = 0;
		if (array[i][j] == '-' || array[i][j] == '+')
			j++;
		while (array[i][j])
		{
			if (array[i][j] < '0' || array[i][j] > '9')
				error_msg("numeric arguments required\n", NULL, NULL, 0);
			j++;
		}
		i++;
	}
}

void	init_philo(t_thread *philo, int i, t_data *d)
{
	philo->position = i;
	philo->nb_eat = d->nb_eat;
	philo->life = 1;
	philo->thread_id = 0;
	philo->fork_status = 0;
	philo->status = NEUTRAL;
	philo->data = d;
	philo->next = NULL;
}

t_thread	*init_thread_struc(t_data *d, t_thread *philo, unsigned int i)
{
	t_thread		*begin;
	t_thread		*previous;

	previous = NULL;
	while (i <= d->philo)
	{
		philo = malloc (sizeof (t_thread));
		if (!philo)
			error_msg("malloc error: check computer's memory\n", d, philo, 1);
		init_philo(philo, i, d);
		philo->previous = previous;
		if (previous)
			previous->next = philo;
		else
			begin = philo;
		previous = philo;
		i++;
	}
	previous->next = begin;
	begin->previous = previous;
	return (begin);
}

t_data	*initialize_struc(t_data *data, char **array, int i)
{
	unsigned int	tmp;

	data = malloc(sizeof (t_data));
	if (!data)
		error_msg("malloc error: check computer's memory\n", NULL, NULL, 0);
	while (array[i])
	{
		tmp = unsigned_atoi(array[i], data);
		if (i == 1 && (tmp < 1 || tmp > 200))
			error_msg("arg 1: put a number between 1 & 200\n", data, NULL, 0);
		if (i == 1)
			data->philo = tmp;
		else if (i == 2)
			data->to_die = tmp;
		else if (i == 3)
			data->to_eat = tmp;
		else if (i == 4)
			data->to_sleep = tmp;
		else if (i == 5)
			data->nb_eat = tmp;
		i++;
	}
	gettimeofday(&data->start_time, NULL);
	data->threads = NULL;
	return (data);
}

int	main(int argc, char **argv)
{
	t_data		*data;

	data = NULL;
	if (argc < 5 || argc > 6)
		error_msg("program need four or five arguments\n", NULL, NULL, 0);
	check_args(argv);
	data = initialize_struc(data, argv, 1);
	if (argc == 5)
		data->nb_eat = -2;
	data->threads = init_thread_struc(data, data->threads, 1);
	if (data->philo == 1)
		do_philo(data->threads, data);
	do_mutex(data);
	do_threads(data);
	free_threads(data->threads, data->philo);
	destroy_mutex(data->threads, data->philo);
	free_philo(data);
	return (0);
}
