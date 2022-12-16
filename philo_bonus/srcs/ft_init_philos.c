/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fi_init_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 09:05:37 by aquincho          #+#    #+#             */
/*   Updated: 2022/12/15 09:05:42 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_init_data_philo(t_table *table, int i)
{
		table->philo->id = i;
		table->philo->is_dead = false;
		table->philo->ate_enough = false;
		table->philo->eat_count = 0;
		table->philo->time_last_meal = 0;
		table->philo->time_left = table->rules->time_die;
}

void	ft_init_philos(t_table *table)
{
	table->msg = (t_msg *)malloc(sizeof(*(table->msg)));
	table->global = (t_global *)malloc(sizeof(*(table->global)));
	if (!table->msg || !table->global)
		ft_err_free_exit("Cannot initialize data", table);
	table->global->repletion = false;
	table->global->is_death = false;
	table->philo = (t_philo *)malloc(sizeof(*(table->philo)));
	if (!table->philo)
		ft_err_free_exit("Cannot initialize philosopher", table);
	table->philo_pid = malloc(sizeof(pid_t) * (table->rules->nbr_philo));
	if (!table->philo_pid)
		ft_err_free_exit("Cannot initialize philosopher pids", table);
}
