/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:49:16 by quackson          #+#    #+#             */
/*   Updated: 2023/04/18 11:33:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	condition_usleep(int msec, t_philo *philo)
{
	int	stop;

	(void) philo;
	pthread_mutex_lock(&philo->info->stop_cond_mutex);
	stop = philo->info->stop_condition == 1;
	pthread_mutex_unlock(&philo->info->stop_cond_mutex);
	if (stop == 0)
		usleep(msec * 1000);
}

int	print_msg(t_philo *philo, const char *msg, int flag)
{
	unsigned long	timestamp;
	int				result;

	result = 0;
	pthread_mutex_lock(&philo->info->print_mutex);
	timestamp = get_timestamp() - philo->start_time;
	if (philo->info->stop_condition == 0)
	{
		printf("%ld %d %s\n", timestamp, philo->id, msg);
		result = 1;
	}
	if (flag == ALIVE)
		pthread_mutex_unlock(&philo->info->print_mutex);
	return (result);
}

void	print_dead(t_philo *philo)
{
	unsigned long	timestamp;

	timestamp = get_timestamp() - philo->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, "died");
}

long long	get_timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000LL + time.tv_usec / 1000LL);
}

void	destroy(t_info *info)
{
	int	i;

	if (!info)
		return ;
	if (info->forks)
	{
		i = -1;
		while (++i < info->num_philos)
		{
			pthread_mutex_destroy(&info->forks[i]);
			pthread_mutex_destroy(&info->philo_list[i].mutex);
		}
		free(info->forks);
	}
	free(info->philo_list);
	pthread_mutex_destroy(&info->stop_cond_mutex);
	pthread_mutex_destroy(&info->print_mutex);
	pthread_mutex_destroy(&info->meal_mutex);
	//pthread_mutex_destroy(&info->print);
	free(info);
}
