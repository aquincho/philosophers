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

void	*ft_philo_supervisor(void *data)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)data;
	philo = table->philo;
	while (!philo->is_dead)
	{
		pthread_mutex_lock(&table->philo->m_is_dead);
		philo->is_dead = (ft_isdead(table) || philo->is_dead);
		pthread_mutex_unlock(&table->philo->m_is_dead);
		if (philo->is_dead)
		{
			ft_display_msg(table, ft_get_time(), TYPE_DIE);
			sem_post(table->global->sem_is_death);
			sem_wait(table->msg->sem_msg);
		}
		pthread_mutex_lock(&table->philo->m_eat_count);
		if (table->rules->nbr_eat > 0
			&& philo->eat_count >= table->rules->nbr_eat && !philo->is_dead
			&& !philo->ate_enough)
		{
			philo->ate_enough = true;
			sem_post(table->global->sem_repletion);
		}
		pthread_mutex_unlock(&table->philo->m_eat_count);
		usleep(10);
	}
	return (NULL);
}

static inline void	ft_kill_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules->nbr_philo)
	{
		kill(table->philo_pid[i], SIGINT);
		i++;
	}
}

void	*ft_repletion_supervisor(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	i = 0;
	while (i < table->rules->nbr_philo + 1)
	{
		sem_wait(table->global->sem_repletion);
		i++;
	}
	pthread_mutex_lock(&table->global->m_repletion);
	table->global->repletion = true;
	pthread_mutex_unlock(&table->global->m_repletion);
	pthread_mutex_lock(&table->global->m_is_death);
	if (!table->global->is_death)
	{
		usleep (table->rules->time_eat * 1000);
		ft_display_msg(table, ft_get_time(),
			TYPE_ENOUGH);
		sem_wait(table->msg->sem_msg);
		sem_post(table->global->sem_is_death);
	}
	pthread_mutex_unlock(&table->global->m_is_death);
	return (NULL);
}

void	*ft_death_supervisor(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	sem_wait(table->global->sem_is_death);
	ft_kill_philos(table);
	pthread_mutex_lock(&table->global->m_is_death);
	table->global->is_death = true;
	pthread_mutex_unlock(&table->global->m_is_death);
	pthread_mutex_lock(&table->global->m_repletion);
	if (!table->global->repletion)
	{
		i = 0;
		while (i < table->rules->nbr_philo + 1)
		{
			sem_post(table->global->sem_repletion);
			i++;
		}
	}
	pthread_mutex_unlock(&table->global->m_repletion);
	return (NULL);
}
