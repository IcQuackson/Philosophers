/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:16:38 by quackson          #+#    #+#             */
/*   Updated: 2023/05/06 20:16:19 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# define DEAD 1
# define ALIVE 0

//#define malloc(x) NULL

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	t_info			*info;
	pthread_mutex_t	*l_mutex;
	pthread_mutex_t	*r_mutex;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	int				id;
	int				times_eaten;
	long long		last_time_eaten;
	long long		start_time;
	t_philo			*philo_list;
}	t_philo;

typedef struct s_info
{
	t_philo			*philo_list;
	pthread_mutex_t	*forks;
	pthread_mutex_t	stop_cond_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	int				stop_condition;
	long long		start_time;
}	t_info;

typedef struct s_data
{
	t_info	*info;
	t_philo	*philo;
}	t_data;

int				ft_atoi(const char *nptr);
int				error(const char *msg, t_info *info);
void			destroy(t_info *info);
long long		get_timestamp(void);
int				print_msg(t_philo *philo, const char *msg, int flag);
void			print_dead(t_philo *philo);
void			check_if_dead(t_info *info, t_philo *philo_list);
void			destroy(t_info *info);
int				philo_eat(t_philo *philo);
void			*philo_routine(void *ptr);
t_info			*init_info(int argc, char **argv);
int				init_forks(t_info *info);
int				init_philos(t_info *info);
int				init_threads(t_info *info);
int				init_mutexes(t_info *info);
void			ft_usleep(int usec);
void			*ft_calloc(size_t nelem, size_t elsize);

#endif