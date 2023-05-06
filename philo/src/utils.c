/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:08:14 by pedgonca          #+#    #+#             */
/*   Updated: 2023/05/06 19:46:50 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_usleep(int msec)
{
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
	pthread_mutex_destroy(&info->print_mutex);
	free(info);
}

int	error(const char *msg, t_info *info)
{
	while (*msg)
		write(1, msg++, 1);
	destroy(info);
	return (0);
}
