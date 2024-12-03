/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:08:57 by esellier          #+#    #+#             */
/*   Updated: 2024/12/01 21:06:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_msg(char *str, t_data *data, t_thread *threads, int mutex)
{
	if (threads)
		free_threads(threads, data->philo);
	if (mutex)
		destroy_mutex(threads, data->philo);
	write (2, str, ft_strlen(str));
	if (data)
		free (data);
	exit (1);
}

unsigned int	unsigned_atoi(const char *str, t_data *data)
{
	long int	i;
	long int	r;
	long int	s;

	i = 0;
	s = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	if (str[i] == '-')
	{
		s = -1;
		i++;
	}
	if (str[i] == '+' && s != -1)
		i++;
	r = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - '0');
		i++;
	}
	r = s * r;
	if (r > UINT_MAX || r < 0)
		error_msg("only unsigned int accepted\n", data, NULL, 0);
	return (r);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len ++;
	return (len);
}

void	free_threads(t_thread *current, unsigned int philo)
{
	t_thread		*next;
	unsigned int	count;
	void			*rtrn;

	count = 0;
	while (current && count < philo)
	{
		next = current->next;
		if (current->thread_id)
		{
			if (pthread_join(current->thread_id, &rtrn) != 0)
				write(2, "error: thread join failed\n", 26);
		}
		current = next;
		count++;
	}
	return ;
}

void	free_philo(t_data *data)
{
	t_thread		*next;
	t_thread		*current;
	unsigned int	count;
	
	count = 0;
	current = data->threads;
	while (current && count < data->philo)
	{
		next = current->next;
		free(current);
		current = next;
		count++;
	}
	free(data);	
}
