/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:28:25 by pedgonca          #+#    #+#             */
/*   Updated: 2023/04/18 12:58:16 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_info	*init_info(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->num_times_to_eat = -1;
	info->stop_condition = 0;
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
		if (pthread_mutex_init(&philo_list[i].mutex, NULL))
			return (0);
		philo_list[i].info = info;
		philo_list[i].id = i + 1;
		philo_list[i].times_eaten = 0;
		philo_list[i].is_done = 0;
		philo_list[i].num_philos = info->num_philos;
		philo_list[i].time_to_die = info->time_to_die;
		philo_list[i].time_to_eat = info->time_to_eat;
		philo_list[i].time_to_sleep = info->time_to_sleep;
		philo_list[i].num_times_to_eat = info->num_times_to_eat;
		/* philo_list[i].l_mutex = &info->forks[i];
		philo_list[i].r_mutex = &info->forks[(i + 1) % info->num_philos]; */
		if (info->num_philos % 2 == 0)
		{
			if (i % 2 == 0)
			{
				philo_list[i].l_mutex = &info->forks[i];
				philo_list[i].r_mutex = &info->forks[(i + 1) % info->num_philos];
			}
			else
			{
				philo_list[i].l_mutex = &info->forks[(i + 1) % info->num_philos];
				philo_list[i].r_mutex = &info->forks[i];
			}
		}
		else
		{
			if (i + 1 != info->num_philos)
			{
				philo_list[i].l_mutex = &info->forks[i];
				philo_list[i].r_mutex = &info->forks[(i + 1) % info->num_philos];
			}
			else
			{
				philo_list[i].l_mutex = &info->forks[(i + 1) % info->num_philos];
				philo_list[i].r_mutex = &info->forks[i];
			}
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
		//printf("created: %d\n", i);
		if (pthread_create(&philo_list[i].thread, NULL, philo_routine,
				&philo_list[i]))
		{
			printf("THREAD FAIL\n");
			return (0);
		}
		usleep(100);
	}
	i = 0;
	check_if_dead(info, info->philo_list);
	printf("--------------------------------------------------------------------\n");
	if (info->stop_condition == 1)
		pthread_mutex_unlock(&info->print_mutex);
	while (++i < info->num_philos)
		pthread_join(info->philo_list[i].thread, NULL);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (!(*nptr == '-' || *nptr == '+' || (*nptr >= '0' && *nptr <= '9')))
		return (0);
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		n *= 10;
		n += *nptr - '0';
		nptr++;
	}
	return (n * sign);
}
