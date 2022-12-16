/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 12:29:48 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/14 12:29:50 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*ft_msg_type(int type)
{
	if (type == 0)
		return ("has taken a fork");
	else if (type == 1)
		return ("is eating");
	else if (type == 2)
		return ("is sleeping");
	else if (type == 3)
		return ("is thinking");
	else if (type == 4)
		return ("died");
	else if (type == 5)
		return ("Philosophers have had enough food!");
	return ("error");
}

void	ft_display_msg(t_table *table, unsigned long long time, int type)
{
	if (!table || !table->philo)
		return ;
	time -= table->global->start_time;
	sem_wait(table->msg->sem_msg);
	printf("%lld\t", time);
	if (type >= 0 && type <= 4)
		printf("%d ", table->philo->id + 1);
	printf("%s\n", ft_msg_type(type));
	if (type != TYPE_DIE && type != TYPE_ENOUGH)
		sem_post(table->msg->sem_msg);
}
