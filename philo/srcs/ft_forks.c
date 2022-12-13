/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_forks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 13:59:21 by aquincho          #+#    #+#             */
/*   Updated: 2022/11/10 13:59:24 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_release_forks(t_philo *philo, bool left, bool right)
{
	if (philo->id % 2 == 0)
	{
		if (left)
			pthread_mutex_unlock(&philo->lfork->m_fork);
		if (right)
			pthread_mutex_unlock(&philo->rfork->m_fork);
	}
	else
	{
		if (right)
			pthread_mutex_unlock(&philo->rfork->m_fork);
		if (left)
			pthread_mutex_unlock(&philo->lfork->m_fork);
	}
}

static bool	ft_fork(t_philo *philo, bool first, bool right)
{
	bool	philo_end;

	if (!right)
		pthread_mutex_lock(&philo->lfork->m_fork);
	else
		pthread_mutex_lock(&philo->rfork->m_fork);
	philo_end = ft_philo_end(philo);
	if (!philo_end)
		ft_display_msg(philo, ft_get_time(), TYPE_FORK);
	else
	{
		if (first == true && (philo->id % 2 == 0))
			ft_release_forks(philo, false, right);
		else if (first == true)
			ft_release_forks(philo, true, right);
		else
			ft_release_forks(philo, true, true);
		return (true);
	}
	return (philo_end);
}

bool	ft_take_forks(t_philo *philo)
{
	bool	philo_end;

	if (philo->id % 2 == 0)
		philo_end = ft_fork(philo, true, true);
	else
		philo_end = ft_fork(philo, true, false);
	if (philo->rules->nbr_philo == 1)
	{
		usleep(philo->rules->time_die * 1000);
		ft_release_forks(philo, false, true);
		return (true);
	}
	if (!philo_end)
	{
		if (philo->id % 2 == 0)
			philo_end = ft_fork(philo, false, false);
		else
			philo_end = ft_fork(philo, false, true);
	}
	return (philo_end);
}
