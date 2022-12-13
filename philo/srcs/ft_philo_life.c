/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_life.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 10:11:19 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/18 10:11:22 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_philo_end(t_philo *philo)
{
	bool	philo_end;

	pthread_mutex_lock(&philo->m_is_dead);
	pthread_mutex_lock(&philo->global->m_is_death);
	pthread_mutex_lock(&philo->global->m_all_ate_enough);
	philo_end = (philo->is_dead || philo->global->is_death
			|| philo->global->all_ate_enough);
	pthread_mutex_unlock(&philo->global->m_all_ate_enough);
	pthread_mutex_unlock(&philo->global->m_is_death);
	pthread_mutex_unlock(&philo->m_is_dead);
	return (philo_end);
}

static void	*ft_philo_loop(t_philo *philo)
{
	bool	philo_end;

	philo_end = false;
	while (!philo_end)
	{
			philo_end = ft_take_forks(philo);
		if (!philo_end)
			philo_end = ft_eat(philo);
		if (!philo_end)
			philo_end = ft_sleep(philo);
		if (!philo_end)
			philo_end = ft_think(philo, (philo->rules->time_die
						- philo->rules->time_eat
						- philo->rules->time_sleep) / 3,
					false);
		philo_end = ft_philo_end(philo);
	}
	return (NULL);
}

static void	*ft_philo_life(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->m_time_last_meal);
	philo->time_last_meal = philo->global->start_time;
	pthread_mutex_unlock(&philo->m_time_last_meal);
	if (philo->id % 2 == 1)
		ft_think(philo, philo->rules->time_eat / 2, true);
	ft_philo_loop(philo);
	return (NULL);
}

int	ft_philo_create(t_table *table)
{
	int		result;
	int		i;

	result = 0;
	table->global->start_time = ft_get_time();
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		result = pthread_create(&(table->philo[i].thr_id),
				NULL, &ft_philo_life, (void *)(&table->philo[i]));
		if (result)
			result = ft_err_free_exit("Cannot create philosopher...", table);
		i++;
	}
	return (result);
}
