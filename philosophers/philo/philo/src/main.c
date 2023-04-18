/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quackson <quackson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:55:04 by quackson          #+#    #+#             */
/*   Updated: 2023/04/17 11:42:38 by quackson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* int	check_if_starved(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_lock(&info->philo_list[i].times_eaten_mutex);
		if (info->philo_list[i].times_eaten == info->num_times_to_eat)
			i++;
		pthread_mutex_unlock(&info->philo_list[i].times_eaten_mutex);
		else
			break ;
	}
	return (i == info->num_philos);
} */

int	philo_finished(t_info *info)
{
	int	i;
	int	n;

	if (info->num_times_to_eat == -1)
		return (0);
	n = 0;
	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_lock(&info->philo_list[i].mutex);
		if (info->philo_list[i].is_done)
			n++;
		pthread_mutex_unlock(&info->philo_list[i].mutex);
		i++;
	}
	return (n == info->num_philos);
}

void	check_if_dead(t_info *info, t_philo *philo_list)
{
	int				i;
	//int				n;

	while (1)
	{
		i = -1;
		//n = 0;
		while (++i < info->num_philos && !info->stop_condition)
		{
			pthread_mutex_lock(&philo_list[i].mutex);
			/* if (philo_list[i].times_eaten == info->num_times_to_eat)
				n++;
			else  */if ((int)(get_timestamp() - philo_list[i].last_time_eaten)
				>= info->time_to_die)
			{
				print_msg(&philo_list[i], "died", DEAD);
				pthread_mutex_lock(&info->stop_cond_mutex);
				info->stop_condition = 1;
				pthread_mutex_unlock(&info->stop_cond_mutex);
			}
			pthread_mutex_unlock(&philo_list[i].mutex);
		}
		if (info->stop_condition == 1/*  || n == info->num_philos */)
			return ;
	}
	printf("end\n");
}

/* void	*check_if_dead(void *ptr)
{
	unsigned long	start;
	t_philo			*philo;
	t_info			*info;

	philo = (t_philo *) ptr;
	info = philo->info;
	while (1)
	{
		pthread_mutex_lock(&philo->mutex);
		start = philo->last_time_eaten;
		if ((int)(get_timestamp() - start) >= philo->time_to_die)
		{
			pthread_mutex_lock(&info->stop_cond_mutex);
			if (info->stop_condition == 0)
				print_msg(philo, "died");
			info->stop_condition = 1;
			pthread_mutex_unlock(&info->stop_cond_mutex);
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutex);
	}
	return (NULL);
} */

int	error(const char *msg, t_info *info)
{
	while (*msg)
		write(1, msg++, 1);
	destroy(info);
	return (0);
}

/* int	philo_eat(t_philo *philo)
{
	int	result;

	result = 1;
	pthread_mutex_lock(&philo->mutex);
	pthread_mutex_lock(philo->l_mutex);
	result = print_msg(philo, "has taken a fork 1");
	pthread_mutex_lock(philo->r_mutex);
	result = print_msg(philo, "has taken a fork 2");
	result = print_msg(philo, "is eating");
	//printf("%d result = %d\n", philo->id, result);
	condition_usleep(philo->time_to_eat, philo);
	philo->last_time_eaten = get_timestamp();
	philo->times_eaten++;
	pthread_mutex_unlock(philo->r_mutex);
	pthread_mutex_unlock(philo->l_mutex);
	pthread_mutex_unlock(&philo->mutex);
	return (result);
} */

int	philo_eat(t_philo *philo)
{
	int	result;

	result = 1;
	pthread_mutex_lock(philo->l_mutex);
	result = print_msg(philo, "has taken a fork", ALIVE);
	pthread_mutex_lock(philo->r_mutex);
	result = print_msg(philo, "has taken a fork", ALIVE);
	pthread_mutex_lock(&philo->mutex);
	result = print_msg(philo, "is eating", ALIVE);
	philo->last_time_eaten = get_timestamp();
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex);
	condition_usleep(philo->time_to_eat, philo);
	pthread_mutex_unlock(philo->r_mutex);
	pthread_mutex_unlock(philo->l_mutex);
	return (result);
}

void	ft_usleep(int ms)
{
	long int	time;

	time = get_timestamp();
	while (get_timestamp() - time < ms)
		usleep(ms / 10);
}

void	*philo_routine(void *ptr)
{
	t_philo			*philo;

	philo = (t_philo *) ptr;
	printf("%lld %d %s\n", get_timestamp() - philo->start_time, philo->id, "iniciou");
	while (philo->times_eaten != philo->num_times_to_eat)
	{
		/* if (philo->id % 2)
			ft_usleep(philo->time_to_eat / 10); */
		if (philo_eat(philo) == 0)
		{
			//printf("FAIL\n");
			break ;
		}
		print_msg(philo, "is sleeping", ALIVE);
		condition_usleep(philo->time_to_sleep, philo);
		print_msg(philo, "is thinking", ALIVE);
	}
	//printf("%d END!\n", philo->id);
	return (NULL);
}

void	*meal_check_routine(void *ptr)
{
	t_info	*info;
	int		i;

	info = (t_info *) ptr;
	while (1)
	{
		i = 0;
		while (i < info->num_philos)
		{
			pthread_mutex_lock(&info->philo_list[i].mutex);
			if (info->num_times_to_eat == info->philo_list[i].times_eaten)
				i++;
			else
			{
				pthread_mutex_unlock(&info->philo_list[i].mutex);
				break ;
			}
			pthread_mutex_unlock(&info->philo_list[i].mutex);
		}
		if (i == info->num_philos)
		{
			/* pthread_mutex_lock(&info->stop_cond_mutex);
			info->stop_condition = 1;
			pthread_mutex_unlock(&info->stop_cond_mutex); */
			break ;
		}
	}
	return (NULL);
}


int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc != 5 && argc != 6)
		return (0);
	info = init_info(argc, argv);
	if (!info)
		return (0);
	if (!init_mutexes(info))
	{
		free(info);
		return (0);
	}
	if (!init_forks(info))
		destroy(info);
	if (!init_philos(info) || !init_threads(info))
		destroy(info);
	/* if (info->stop_condition == 1)
		pthread_mutex_unlock(&info->print); */
	destroy(info);
	return (0);
}
