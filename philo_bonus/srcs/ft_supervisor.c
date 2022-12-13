/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 10:11:48 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/18 10:11:50 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static inline void	ft_dead_philo_action(t_table *table)
{
	ft_display_msg(table, ft_get_time(), TYPE_DIE);
	table->philo->is_end = true;
	table->philo->is_dead = true;
	sem_post(table->global->m_is_death);
	sem_post(table->global->m_is_end);
}

void	*ft_philo_supervisor(void *data)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)data;
	philo = table->philo;
	while (!philo->is_dead && !philo->is_end)
	{
		philo->is_dead = (ft_isdead(table) || philo->is_dead);
		if (philo->is_dead && !philo->is_end)
			ft_dead_philo_action(table);
		if (table->rules->nbr_eat > 0
			&& philo->eat_count >= table->rules->nbr_eat && !philo->is_dead
			&& !philo->ate_enough && !philo->is_end)
		{
			sem_post(table->global->m_repletion);
			philo->ate_enough = true;
		}
		usleep(10);
	}
	return (NULL);
}

void	*ft_end_philo_supervisor(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	sem_wait(table->global->m_is_end);
	table->philo->is_end = true;
	sem_post(table->global->m_is_end);
	return (NULL);
}

void	*ft_repletion_supervisor(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	i = 0;
	while (i < table->rules->nbr_philo + 1)
	{
		sem_wait(table->global->m_repletion);
		i++;
	}
	if (!table->global->is_death)
	{
		usleep (table->rules->time_eat * 1000);
		ft_display_msg(table, ft_get_time(),
			TYPE_ENOUGH);
		sem_wait(table->msg->m_msg);
	}
	sem_post(table->global->m_is_death);
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		sem_post(table->global->m_is_end);
		i++;
	}
	return (NULL);
}

void	*ft_death_supervisor(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	sem_wait(table->global->m_is_death);
	table->global->is_death = true;
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		sem_post(table->global->m_is_end);
		i++;
	}
	i = 0;
	while (i < table->rules->nbr_philo + 1)
	{
		sem_post(table->global->m_repletion);
		i++;
	}
	return (NULL);
}
