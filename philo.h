/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:27:29 by esellier          #+#    #+#             */
/*   Updated: 2024/12/07 17:04:59 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <pthread.h>

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DEAD 4
# define NEUTRAL 5
# define END 6

typedef struct s_data	t_data;
typedef struct s_thread	t_thread;

typedef struct s_thread
{
	unsigned int		position;
	int					nb_eat;
	struct timeval		start_time;
	int					life;
	pthread_t			thread_id;
	pthread_mutex_t		fork;
	int					fork_status;
	int					status;
	t_data				*data;
	struct s_thread		*previous;
	struct s_thread		*next;
}				t_thread;

typedef struct s_data
{
	unsigned int		philo;
	struct timeval		start_time;
	pthread_t			thread_die;
	pthread_mutex_t		status_mutex;
	pthread_mutex_t		general_mutex;
	pthread_mutex_t		print_mutex;
	unsigned long		to_die;
	unsigned long		to_eat;
	unsigned long		to_sleep;
	int					nb_eat;
	t_thread			*threads;
}				t_data;

//main
void			check_args(char **array);
void			init_philo(t_thread *philo, int i, t_data *d);
t_thread		*init_thread_struc(t_data *data, t_thread *philo,
					unsigned int i);
t_data			*initialize_struc(t_data *data, char **array, int i);
int				main(int argc, char **argv);

//philo_utils
int				error_msg(char *str, t_data *data, t_thread *threads,
					int mutex);
unsigned int	unsigned_atoi(const char *str, t_data *data);
int				ft_strlen(char *str);
void			free_threads(t_thread *thread, unsigned int philo);
void			free_philo(t_data *data);

//create_routine
int				do_eat(t_thread *philo);
int				do_eat_end(t_thread *philo);
int				do_sleep(t_thread *philo);
int				do_think(t_thread *philo);
void			*do_routine(void *arg);

//routine_utils
unsigned long	calcul_time(struct timeval start);
void			*check_die(void *arg);
void			*do_die(t_data *d, t_thread *philo);
void			print_eat(unsigned long time, t_thread *philo);
int				print_status(t_thread *philo, int i, unsigned long time);

//mutex
ssize_t			do_threads(t_data *data);
void			do_mutex(t_data *data);
void			*destroy_mutex(t_thread *current, unsigned int philo);
void			do_philo(t_thread *philo, t_data *data);

#endif