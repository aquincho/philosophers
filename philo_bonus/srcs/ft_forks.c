/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_forks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 09:34:15 by aquincho          #+#    #+#             */
/*   Updated: 2022/12/15 09:34:19 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_release_forks(t_table *table)
{
	sem_post(table->sem_fork);
	sem_post(table->sem_fork);
}

bool	ft_take_forks(t_table *table)
{
	bool	philo_end;

	philo_end = ft_death_check(table);
	if (philo_end)
		return (true);
	sem_wait(table->sem_fork);
	ft_display_msg(table, ft_get_time(), TYPE_FORK);
	if (table->rules->nbr_philo == 1)
	{
		usleep(table->rules->time_die * 1000);
		sem_post(table->sem_fork);
		return (true);
	}
	sem_wait(table->sem_fork);
	ft_display_msg(table, ft_get_time(), TYPE_FORK);
	sem_post(table->global->sem_take_forks);
	return (philo_end);
}
