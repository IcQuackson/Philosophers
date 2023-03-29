/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quackson <quackson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:55:04 by quackson          #+#    #+#             */
/*   Updated: 2023/03/29 00:11:14 by quackson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	destroy(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_philos)
		pthread_mutex_destroy(&(info->philo_list[i].mutex));
	free(info->philo_list);
	free(info);
}

int	error(const char *msg, t_info *info)
{
	while (*msg)
		write(1, msg++, 1);
	destroy(info);
	return (0);
}

int	load_info(t_info **info, int argc, char **argv)
{
	*info = malloc(sizeof(t_info));
	if (!*info)
		return (0);
	(*info)->num_philos = ft_atoi(argv[1]);
	(*info)->time_to_die = ft_atoi(argv[2]);
	(*info)->time_to_eat = ft_atoi(argv[3]);
	(*info)->time_to_sleep = ft_atoi(argv[4]);
	(*info)->num_times_to_eat = 0;
	if (argc == 6)
	{
		(*info)->num_times_to_eat = ft_atoi(argv[5]);
		if ((*info)->num_times_to_eat == 0)
			return (0);
	}
	return ((*info)->num_philos > 0 && (*info)->time_to_die > 0
		&& (*info)->time_to_eat > 0 && (*info)->time_to_sleep > 0
		&& (*info)->num_times_to_eat >= 0);
}

void	grab_fork(pthread_mutex_t mutex1, pthread_mutex_t mutex2,
	t_philo *philo)
{
	pthread_mutex_lock(&mutex1);
	pthread_mutex_lock(&mutex2);
	printf("%d has taken a fork\n", philo->id);
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
}

void	*routine(void *ptr)
{
	t_philo			*philo;
	t_info			*info;
	pthread_mutex_t	mutex1;
	pthread_mutex_t	mutex2;

	philo = (t_philo *) ptr;
	info = philo->info;
	printf("id:%d\n", philo->id);
	printf("left_id: %d right_id:%d\n", info->philo_list[philo->id - 2 % info->num_philos].id,
		info->philo_list[philo->id % info->num_philos].id);
	mutex1 = info->philo_list[philo->id - 2 % info->num_philos].mutex;
	mutex2 = info->philo_list[philo->id % info->num_philos].mutex;
	while (philo->times_eaten--)
	{
		grab_fork(mutex1, mutex2, philo);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_info	*info;
	t_philo	*philo_list;
	int		i;

	if (argc != 5 && argc != 6)
		return (0);
	info = NULL;
	if (!load_info(&info, argc, argv))
		return (error("ERROR: load_info\n", info));
	printf("%d %d %d %d %d\n", info->num_philos, info->time_to_die, info->time_to_eat, info->time_to_die, info->num_times_to_eat);
	info->philo_list = malloc(info->num_philos * sizeof(t_philo));
	if (info->philo_list == NULL)
		return (error("ERROR: allocating philo_list", info));
	i = 0;
	philo_list = info->philo_list;
	while (i < info->num_philos)
	{
		pthread_mutex_init(&(philo_list[i].mutex), NULL);
		philo_list[i].id = i + 1;
		philo_list[i].times_eaten = 0;
		philo_list[i].info = info;
		pthread_create(&(philo_list[i].thread), NULL, routine, &philo_list[i]);
		i++;
	}
	i = -1;
	while (++i < info->num_philos)
		pthread_join(info->philo_list[i].thread, NULL);
	destroy(info);
	return (0);
}
