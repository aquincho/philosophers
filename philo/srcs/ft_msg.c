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

#include "philo.h"

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
	return ("error\n");
}

void	ft_display_msg(t_philo *philo, unsigned long long time, int type)
{
	pthread_mutex_lock (&philo->global->m_is_end);
	if (!philo || (philo->global->is_end && !(type == 4 || type == 5)))
	{
		pthread_mutex_unlock (&philo->global->m_is_end);
		return ;
	}
	pthread_mutex_lock (&philo->msg->m_msg);
	if (type == 4 || type == 5)
		time -= philo->global->start_time;
	else
		time = ft_get_time() - philo->global->start_time;
	if (!philo->global->is_end
		|| (philo->global->is_end && (type == 4 || type == 5)))
	{
		printf("%lld ", time);
		if (type >= 0 && type <= 4)
			printf("%d ", philo->id + 1);
		printf("%s\n", ft_msg_type(type));
	}
	pthread_mutex_unlock (&philo->global->m_is_end);
	pthread_mutex_unlock (&philo->msg->m_msg);
}
