/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quackson <quackson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:16:38 by quackson          #+#    #+#             */
/*   Updated: 2023/03/29 00:08:44 by quackson         ###   ########.fr       */
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

typedef struct s_philo
{
	t_info			*info;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	int				id;
	int				times_eaten;
	struct timeval	last_time_eaten;
	int				num_times_to_eat;
}	t_philo;

typedef struct s_info
{
	t_philo		*philo_list;
	int			num_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_times_to_eat;
}	t_info;

typedef struct s_data
{
	t_info	*info;
	t_philo	*philo;
}	t_data;

int		ft_atoi(const char *nptr);
int		load_info(t_info **info, int argc, char **argv);
int		error(const char *msg, t_info *info);
void	destroy(t_info *info);
char	*ft_itoa(int n);

#endif