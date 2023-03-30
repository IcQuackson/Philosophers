/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:16:38 by quackson          #+#    #+#             */
/*   Updated: 2023/03/30 18:45:26 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>

# include <stdio.h>

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	pthread_mutex_t	mutex;
	pthread_t		thread;
	int				id;
	int				times_eaten;
	unsigned long	last_time_eaten;

	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	int				has_eaten;
	unsigned long	start_time;
	t_philo			*philo_list;
}	t_philo;

typedef struct s_info
{
	t_philo			*philo_list;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	struct timeval	start_time;
}	t_info;

typedef struct s_data
{
	t_info	*info;
	t_philo	*philo;
}	t_data;

int				ft_atoi(const char *nptr);
int				load_info(t_info **info, int argc, char **argv);
int				error(const char *msg, t_info *info);
void			destroy(t_info *info);
char			*ft_itoa(int n);
void			ft_putnbr(long n);
unsigned long	get_timestamp(void);
void			ft_putstr(const char *str);

#endif