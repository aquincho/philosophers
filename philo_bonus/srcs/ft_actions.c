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

#include "philo_bonus.h"

bool	ft_death_check(t_table *table)
{
	bool	philo_end;

	pthread_mutex_lock(&table->philo->m_is_dead);
	philo_end = (table->philo->is_dead);
	pthread_mutex_unlock(&table->philo->m_is_dead);
	return (philo_end);
}

static inline void	ft_eat_time(t_table *table)
{
	pthread_mutex_lock(&table->philo->m_time_last_meal);
	table->philo->time_last_meal = ft_get_time();
	pthread_mutex_unlock(&table->philo->m_time_last_meal);
	usleep(table->rules->time_eat * 1000);
	pthread_mutex_lock(&table->philo->m_eat_count);
	table->philo->eat_count++;
	pthread_mutex_unlock(&table->philo->m_eat_count);
}

bool	ft_eat(t_table *table)
{
	bool	philo_end;
	int		time_left;

	philo_end = ft_death_check(table);
	if (philo_end)
	{
		ft_release_forks(table);
		return (true);
	}
	time_left = ft_time_left(table);
	ft_display_msg(table, ft_get_time(), TYPE_EAT);
	if (time_left >= 0
		&& table->rules->time_die >= table->rules->time_eat)
	{
		ft_eat_time(table);
		philo_end = false;
	}
	else
	{
		philo_end = true;
		usleep(table->philo->time_left * 1000);
	}
	ft_release_forks(table);
	return (philo_end);
}

static bool	ft_think(t_table *table)
{
	bool	philo_end;

	philo_end = ft_death_check(table);
	if (!philo_end)
	{
		ft_display_msg(table, ft_get_time(), TYPE_THINK);
		usleep(((table->rules->time_die - table->rules->time_eat
					- table->rules->time_sleep) / 4) * 1000);
	}
	return (philo_end);
}

bool	ft_sleep(t_table *table)
{
	bool	philo_end;
	int		time_left;

	time_left = ft_time_left(table);
	philo_end = ft_death_check(table);
	if (philo_end)
		return (true);
	ft_display_msg(table, ft_get_time(), TYPE_SLEEP);
	if (time_left >= table->rules->time_sleep)
	{
		philo_end = false;
		usleep(table->rules->time_sleep * 1000);
	}
	else
	{
		philo_end = true;
		usleep(time_left * 1000);
	}
	philo_end = ft_think(table);
	return (philo_end);
}
