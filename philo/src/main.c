/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 12:08:04 by pedgonca          #+#    #+#             */
/*   Updated: 2023/05/06 19:03:34 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	destroy(info);
	return (0);
}

void	*ft_calloc(size_t nelem, size_t elsize)
{
	void	*p;

	p = malloc(nelem * elsize);
	if (p != NULL)
		memset(p, 0, nelem * elsize);
	return (p);
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
