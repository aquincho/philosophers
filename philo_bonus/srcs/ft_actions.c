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

static void	ft_release_forks(t_table *table)
{
	sem_post(table->m_fork);
	sem_post(table->m_fork);
}

bool	ft_take_forks(t_table *table)
{
	t_philo	*philo;
	bool	philo_end;

	philo = table->philo;
	philo_end = (philo->is_dead || philo->is_end);
	if (philo_end)
		return (true);
	sem_wait(table->m_fork);
	ft_display_msg(table, ft_get_time(), TYPE_FORK);
	if (table->rules->nbr_philo == 1)
	{
		usleep(table->rules->time_die * 1000);
		sem_post(table->m_fork);
		return (true);
	}
	sem_wait(table->m_fork);
	ft_display_msg(table, ft_get_time(), TYPE_FORK);
	sem_post(table->global->m_take_forks);
	return (philo_end);
}

static bool	ft_time_sleep(t_table *table, int time, bool is_end)
{
	table->philo->is_dead = is_end;
	usleep (time * 1000);
	return (is_end);
}

bool	ft_eat(t_table *table)
{
	t_philo	*philo;
	bool	philo_end;

	philo = table->philo;
	philo_end = (philo->is_dead || philo->is_end);
	if (philo_end)
	{
		ft_release_forks(table);
		return (true);
	}
	philo->time_left = ft_time_left(table);
	ft_display_msg(table, ft_get_time(), TYPE_EAT);
	if (philo->time_left >= 0
		&& table->rules->time_die >= table->rules->time_eat)
	{
		philo->time_last_meal = ft_get_time();
		philo->eat_count++;
		philo_end = ft_time_sleep(table, table->rules->time_eat, false);
	}
	else
		philo_end = ft_time_sleep(table, philo->time_left, true);
	ft_release_forks(table);
	return (philo_end);
}

bool	ft_sleep(t_table *table)
{
	t_philo	*philo;
	bool	philo_end;

	philo = table->philo;
	philo->time_left = ft_time_left(table);
	philo_end = (philo->is_dead || philo->is_end);
	if (philo_end)
		return (true);
	ft_display_msg(table, ft_get_time(), TYPE_SLEEP);
	if (philo->time_left >= table->rules->time_sleep)
		philo_end = ft_time_sleep(table, table->rules->time_sleep, false);
	else
		philo_end = ft_time_sleep(table, philo->time_left, true);
	if (!philo_end)
		philo_end = (philo->is_dead || philo->is_end);
	if (!philo_end)
	{
		ft_display_msg(table, ft_get_time(), TYPE_THINK);
		usleep(((table->rules->time_die - table->rules->time_eat
					- table->rules->time_sleep) / 4) * 1000);
	}
	return (philo_end);
}
