/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:04:53 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/13 15:04:55 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	ft_free_semaphore(t_table *table)
{
	sem_close(table->msg->sem_msg);
	sem_close(table->global->sem_take_forks);
	sem_close(table->sem_fork);
	sem_close(table->global->sem_repletion);
	sem_close(table->global->sem_is_death);
	sem_unlink("/pmsg");
	sem_unlink("/ptake");
	sem_unlink("/pfork");
	sem_unlink("/prepletion");
	sem_unlink("/pdeath");
}

static void	ft_free_mutex(t_table *table)
{
	pthread_mutex_destroy(&table->global->m_is_death);
	pthread_mutex_destroy(&table->philo->m_is_dead);
	pthread_mutex_destroy(&table->philo->m_time_last_meal);
	pthread_mutex_destroy(&table->philo->m_eat_count);
}

void	ft_free_table(t_table *table)
{
	if (!table)
		return ;
	if (table->semaphore_errinit == 0)
		ft_free_semaphore(table);
	if (table->mutex_errinit == 0)
		ft_free_mutex(table);
	if (table->msg)
		free (table->msg);
	if (table->global)
		free (table->global);
	if (table->rules)
		free (table->rules);
	if (table->philo)
		free (table->philo);
	free (table);
}
