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

static inline void	ft_check_repletion(t_table *table)
{
	t_philo	*philo;

	philo = table->philo;
	pthread_mutex_lock(&table->philo->m_eat_count);
	if (table->rules->nbr_eat > 0
		&& philo->eat_count >= table->rules->nbr_eat && !philo->is_dead
		&& !philo->ate_enough)
	{
		philo->ate_enough = true;
		sem_post(table->global->sem_repletion);
	}
	pthread_mutex_unlock(&table->philo->m_eat_count);
}

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
			return (NULL);
		}
		ft_check_repletion(table);
		usleep(10);
	}
	return (NULL);
}

void	ft_kill_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules->nbr_philo)
	{
		if (kill(table->philo_pid[i], SIGINT) < 0)
			kill(table->philo_pid[i], SIGKILL);
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
		pthread_mutex_unlock(&table->global->m_is_death);
		ft_display_msg(table, ft_get_time(), TYPE_ENOUGH);
		ft_kill_philos(table);
		sem_post(table->global->sem_is_death);
	}
	else
		pthread_mutex_unlock(&table->global->m_is_death);
	return (NULL);
}

void	*ft_death_supervisor(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	sem_wait(table->global->sem_is_death);
	pthread_mutex_lock(&table->global->m_is_death);
	table->global->is_death = true;
	pthread_mutex_unlock(&table->global->m_is_death);
	pthread_mutex_lock(&table->global->m_repletion);
	if (!table->global->repletion)
	{
		pthread_mutex_unlock(&table->global->m_repletion);
		ft_kill_philos(table);
		i = 0;
		while (i < table->rules->nbr_philo + 1)
		{
			sem_post(table->global->sem_repletion);
			i++;
		}
	}
	else
		pthread_mutex_unlock(&table->global->m_repletion);
	return (NULL);
}
