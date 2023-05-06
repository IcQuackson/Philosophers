/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:37:24 by pedgonca          #+#    #+#             */
/*   Updated: 2023/05/06 20:02:38 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philo_routine(void *ptr)
{
	t_philo			*philo;

	philo = (t_philo *) ptr;
	while (philo->times_eaten != philo->info->num_times_to_eat)
	{
		if (philo_eat(philo) == 0)
			break ;
		print_msg(philo, "is sleeping", ALIVE);
		ft_usleep(philo->info->time_to_sleep);
		print_msg(philo, "is thinking", ALIVE);
	}
	return (NULL);
}

int	take_forks(t_philo *philo)
{
	int	result;

	if (philo->id % 2 != 0)
	{
		usleep(philo->info->time_to_eat / 10);
	}
	pthread_mutex_lock(philo->l_mutex);
	result = print_msg(philo, "has taken a fork", ALIVE);
	pthread_mutex_lock(philo->r_mutex);
	result = print_msg(philo, "has taken a fork", ALIVE);
	return (result);
}

int	philo_eat(t_philo *philo)
{
	int	result;

	if (philo->info->num_philos == 1)
	{
		pthread_mutex_lock(philo->l_mutex);
		result = print_msg(philo, "has taken a fork", ALIVE);
		pthread_mutex_unlock(philo->l_mutex);
		return (0);
	}
	result = 1;
	result = take_forks(philo);
	pthread_mutex_lock(&philo->mutex);
	result = print_msg(philo, "is eating", ALIVE);
	philo->last_time_eaten = get_timestamp();
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex);
	ft_usleep(philo->info->time_to_eat);
	pthread_mutex_unlock(philo->l_mutex);
	pthread_mutex_unlock(philo->r_mutex);
	usleep(500);
	return (result);
}

void	check_if_dead(t_info *info, t_philo *philo_list)
{
	int				i;
	int				n;

	while (1)
	{
		i = -1;
		n = 0;
		while (++i < info->num_philos && !info->stop_condition)
		{
			pthread_mutex_lock(&philo_list[i].mutex);
			if (philo_list[i].times_eaten == info->num_times_to_eat)
				n++;
			else if ((int)(get_timestamp() - philo_list[i].last_time_eaten)
			>= info->time_to_die)
			{
				print_msg(&philo_list[i], "died", DEAD);
				info->stop_condition = 1;
			}
			pthread_mutex_unlock(&philo_list[i].mutex);
		}
		if (info->stop_condition == 1 || n == info->num_philos)
			return ;
	}
}
