/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:22:55 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/14 15:22:58 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

unsigned long long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_time_left(t_table *table)
{
	int	time_spent;
	int	time_left;

	time_spent = ft_get_time() - table->philo->time_last_meal;
	time_left = table->rules->time_die - time_spent;
	return (time_left);
}

bool	ft_isdead(t_table *table)
{
	table->philo->time_left = ft_time_left(table);
	if (table->philo->time_left <= 0 || table->philo->is_dead)
	{
		table->philo->is_dead = true;
		return (true);
	}
	return (false);
}
