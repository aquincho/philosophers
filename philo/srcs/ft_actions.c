/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 10:11:32 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/18 10:11:35 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eat_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->m_time_last_meal);
	philo->time_last_meal = ft_get_time();
	pthread_mutex_unlock(&philo->m_time_last_meal);
	usleep(philo->rules->time_eat * 1000);
	pthread_mutex_lock(&philo->m_eat_count);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->m_eat_count);
}

bool	ft_eat(t_philo *philo)
{
	bool	philo_end;
	int		time_left;

	philo_end = ft_philo_end(philo);
	if (philo_end)
	{
		ft_release_forks(philo, true, true);
		return (true);
	}
	time_left = ft_time_left(philo);
	if (time_left >= 0)
		ft_display_msg(philo, ft_get_time(), TYPE_EAT);
	if (time_left >= 0 && philo->rules->time_die >= philo->rules->time_eat)
	{
		ft_eat_count(philo);
		philo_end = false;
	}
	else
	{
		usleep (time_left * 1000);
		philo_end = true;
	}
	ft_release_forks(philo, true, true);
	return (philo_end);
}

bool	ft_sleep(t_philo *philo)
{
	bool	philo_end;

	philo_end = ft_philo_end(philo);
	if (!philo_end)
		ft_display_msg(philo, ft_get_time(), TYPE_SLEEP);
	else
		return (true);
	if (ft_time_left(philo) >= philo->rules->time_sleep)
	{
		usleep(philo->rules->time_sleep * 1000);
		philo_end = false;
	}
	else
	{
		usleep(ft_time_left(philo) * 1000);
		philo_end = true;
	}
	return (philo_end);
}

bool	ft_think(t_philo *philo, int time_think, bool start_even)
{
	bool	philo_end;

	if (!start_even)
		philo_end = ft_philo_end(philo);
	else
		philo_end = false;
	if (!philo_end)
	{
		ft_display_msg(philo, ft_get_time(), TYPE_THINK);
		usleep(time_think * 1000);
	}
	return (philo_end);
}
