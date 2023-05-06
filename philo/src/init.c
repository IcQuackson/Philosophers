/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:09:34 by pedgonca          #+#    #+#             */
/*   Updated: 2023/05/06 20:06:35 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_info	*init_info(int argc, char **argv)
{
	t_info	*info;

	info = ft_calloc(1, sizeof(t_info));
	if (!info)
		return (NULL);
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->num_times_to_eat = -1;
	if (argc == 6)
		info->num_times_to_eat = ft_atoi(argv[5]);
	if (info->num_philos > 0 && info->time_to_die > 0 && info->time_to_eat > 0
		&& info->time_to_sleep > 0 && info->num_times_to_eat >= -1)
	{
		info->forks = malloc(info->num_philos * sizeof(pthread_mutex_t));
		if (!info->forks)
		{
			free(info);
			return (NULL);
		}
		return (info);
	}
	free(info);
	return (NULL);
}

int	init_forks(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_philos)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
			return (0);
	}
	return (1);
}

int	init_philos(t_info *info)
{
	int		i;
	t_philo	*philo_list;

	philo_list = malloc(info->num_philos * sizeof(t_philo));
	if (!philo_list)
		return (0);
	info->philo_list = philo_list;
	i = -1;
	while (++i < info->num_philos)
	{
		pthread_mutex_init(&philo_list[i].mutex, NULL);
		philo_list[i].info = info;
		philo_list[i].id = i + 1;
		philo_list[i].times_eaten = 0;
		philo_list[i].l_mutex = &info->forks[i];
		philo_list[i].r_mutex = &info->forks[(i + 1) % info->num_philos];
		if (i + 1 == info->num_philos)
		{
			philo_list[i].r_mutex = &info->forks[i];
			philo_list[i].l_mutex = &info->forks[(i + 1) % info->num_philos];
		}
	}
	return (1);
}

int	init_mutexes(t_info *info)
{
	if (pthread_mutex_init(&info->stop_cond_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&info->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&info->meal_mutex, NULL))
		return (0);
	return (1);
}

int	init_threads(t_info *info)
{
	int			i;
	t_philo		*philo_list;

	philo_list = info->philo_list;
	i = -1;
	info->start_time = get_timestamp();
	while (++i < info->num_philos)
	{
		info->philo_list[i].start_time = info->start_time;
		info->philo_list[i].last_time_eaten = info->start_time;
		if (pthread_create(&philo_list[i].thread, NULL, philo_routine,
				&philo_list[i]))
		{
			printf("THREAD FAIL\n");
			return (0);
		}
		usleep(100);
	}
	i = -1;
	check_if_dead(info, info->philo_list);
	if (info->stop_condition == 1)
		pthread_mutex_unlock(&info->print_mutex);
	while (++i < info->num_philos)
		pthread_join(info->philo_list[i].thread, NULL);
	return (1);
}
