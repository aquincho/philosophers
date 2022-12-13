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
	sem_close(table->msg->m_msg);
	sem_close(table->global->m_take_forks);
	sem_close(table->m_fork);
	sem_close(table->global->m_repletion);
	sem_close(table->global->m_is_death);
	sem_close(table->global->m_is_end);
	sem_unlink("/pmsg");
	sem_unlink("/ptake");
	sem_unlink("/pfork");
	sem_unlink("/prepletion");
	sem_unlink("/pdeath");
	sem_unlink("/pend");
}

void	ft_free_table(t_table *table)
{
	if (!table)
		return ;
	if (table->semaphore_errinit == 0)
		ft_free_semaphore(table);
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
