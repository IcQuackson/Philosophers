/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:55:04 by quackson          #+#    #+#             */
/*   Updated: 2023/03/30 16:25:49 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* void	print_philo(t_philo	*philo)
{
	printf("id:%d times_eaten:%d last_time_eaten:%d\n",
		philo->id, philo->times_eaten, philo->last_time_eaten);
} */

void	print_msg(t_philo *philo, struct timeval *now, const char *msg)
{
	long	timestamp;

	timestamp = get_timestamp(philo->start_time, now);
	printf("%ld %d %s\n", timestamp, philo->id, msg);
}

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
	(*info)->time_to_die = ft_atoi(argv[2]) * 1000;
	(*info)->time_to_eat = ft_atoi(argv[3]) * 1000;
	(*info)->time_to_sleep = ft_atoi(argv[4]) * 1000;
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

int	g = 0;

unsigned long	get_timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * (unsigned long)1000) + (time.tv_usec / 1000));
}

int	check_if_starved(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2,
	t_philo *philo)
{
	struct timeval	start;
	struct timeval	end;

	if (philo->has_eaten)
		start.tv_usec = philo->start_time.tv_sec;
	else
		start.tv_usec = philo->last_time_eaten.tv_usec;
	if (get_timestamp() - start.tv_usec > philo->time_to_die)
	{
		get_timestamp(philo->start_time, NULL);
		printf("%d died", philo->id);
		pthread_mutex_unlock(mutex2);
		pthread_mutex_unlock(mutex1);
		return (0);
	}
	return (1);
}

int	grab_fork(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2,
	t_philo *philo)
{

	pthread_mutex_lock(mutex1);
	print_msg(philo, NULL, "has taken a fork");
	pthread_mutex_lock(mutex2);
	print_msg(philo, NULL, "has taken a fork");
	/* if (check_if_starved(&mutex1, &mutex2, philo) == 0)
		return (0); */
	gettimeofday(&(philo->last_time_eaten), NULL);
	print_msg(philo, &(philo->last_time_eaten), "is eating");
	philo->has_eaten = 1;
	usleep(philo->time_to_eat);
	g++;
	pthread_mutex_unlock(mutex2);
	pthread_mutex_unlock(mutex1);
	return (1);
}

void	*routine(void *ptr)
{
	t_philo			*philo;
	pthread_mutex_t	*mutex1;
	pthread_mutex_t	*mutex2;
	int				num_philos;
	int				id;

	philo = (t_philo *) ptr;
	num_philos = philo->num_philos;
	id = philo->id;
	mutex1 = &(philo->philo_list[id - 2 % num_philos].mutex);
	mutex2 = &(philo->philo_list[id % num_philos].mutex);
	while (philo->times_eaten++ < philo->num_times_to_eat)
	{
		if (!grab_fork(mutex1, mutex2, philo))
			return (NULL);
		print_msg(philo, NULL, "is sleeping");
		usleep(philo->time_to_sleep);
		print_msg(philo, NULL, "is thinking");
	}
	return (NULL);
}

void	initialize_philos(t_info *info)
{
	int		i;
	t_philo	*philo_list;

	philo_list = info->philo_list;
	i = -1;
	while (++i < info->num_philos)
	{
		philo_list[i].philo_list = philo_list;
		pthread_mutex_init(&(philo_list[i].mutex), NULL);
		philo_list[i].id = i + 1;
		philo_list[i].times_eaten = 0;
		philo_list[i].num_philos = info->num_philos;
		philo_list[i].time_to_die = info->time_to_die;
		philo_list[i].time_to_eat = info->time_to_eat;
		philo_list[i].time_to_sleep = info->time_to_sleep;
		philo_list[i].num_times_to_eat = info->num_times_to_eat;
		philo_list[i].has_eaten = 0;
		gettimeofday(&(philo_list[i].start_time), NULL);
	}
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
	//printf("%d %d %d %d %d\n", info->num_philos, info->time_to_die, info->time_to_eat, info->time_to_die, info->num_times_to_eat);
	info->philo_list = malloc(info->num_philos * sizeof(t_philo));
	//printf("info->num_philos:%d die:%d\n", info->num_philos, info->time_to_die);
	if (info->philo_list == NULL)
		return (error("ERROR: allocating philo_list", info));
	i = 0;
	philo_list = info->philo_list;
	gettimeofday(&(info->start_time), NULL);
	initialize_philos(info);
	while (i < info->num_philos)
	{
		pthread_create(&(philo_list[i].thread), NULL, routine, &philo_list[i]);
		i++;
	}
	i = -1;
	while (++i < info->num_philos)
		pthread_join(info->philo_list[i].thread, NULL);
	destroy(info);
	printf("g:%d\n", g);
	return (0);
}
