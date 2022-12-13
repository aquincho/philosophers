/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:22:55 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/14 15:22:58 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_time_left(t_philo *philo)
{
	int	time_spent;
	int	time_left;

	pthread_mutex_lock(&philo->m_time_last_meal);
	time_spent = ft_get_time() - philo->time_last_meal;
	pthread_mutex_unlock(&philo->m_time_last_meal);
	time_left = philo->rules->time_die - time_spent;
	return (time_left);
}

bool	ft_isdead(t_philo *philo)
{
	if (ft_time_left(philo) <= 0 || philo->is_dead)
	{
		philo->is_dead = true;
		pthread_mutex_lock(&philo->global->m_is_death);
		philo->global->is_death = true;
		pthread_mutex_unlock(&philo->global->m_is_death);
		return (true);
	}
	return (false);
}
