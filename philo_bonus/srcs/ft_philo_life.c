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

#include "philo_bonus.h"

static int	ft_philo_life(t_table *table)
{
	bool	philo_end;
	t_philo	*philo;

	philo = table->philo;
	pthread_mutex_lock(&table->philo->m_time_last_meal);
	philo->time_last_meal = table->global->start_time;
	pthread_mutex_unlock(&table->philo->m_time_last_meal);
	pthread_mutex_lock(&table->philo->m_is_dead);
	philo_end = (philo->is_dead);
	pthread_mutex_unlock(&table->philo->m_is_dead);
	sem_wait(table->global->sem_take_forks);
	while (!philo_end)
	{
		philo_end = ft_take_forks(table);
		if (!philo_end)
			philo_end = ft_eat(table);
		if (!philo_end)
			philo_end = ft_sleep(table);
	}
	return (philo_end);
}

static void	ft_philo_child(t_table *table, int i)
{
	int	result;

	ft_init_data_philo(table, i);
	table->philo->id = i;
	pthread_mutex_lock(&table->philo->m_time_last_meal);
	table->philo->time_last_meal = table->global->start_time;
	pthread_mutex_unlock(&table->philo->m_time_last_meal);
	result = pthread_create(&table->philo->thr_superv, NULL,
			&ft_philo_supervisor, (void *)(table));
	if (result)
		ft_err_free_exit("Cannot supervize philosopher...", table);
	else
		pthread_detach(table->philo->thr_superv);
	result = ft_philo_life(table);
	ft_free_table (table);
	exit (0);
}

static int	ft_philo_threads(t_table *table)
{
	int	result;

	result = 0;
	sem_wait(table->global->sem_is_death);
	result = pthread_create(&table->thr_repletion, NULL,
			&ft_repletion_supervisor, (void *)(table));
	if (result)
		ft_err_free_exit("Cannot supervize philosopher...", table);
	result = pthread_create(&table->thr_death, NULL, &ft_death_supervisor,
			(void *)(table));
	if (result)
		ft_err_free_exit("Cannot supervize death philosopher...", table);
	return (result);
}

int	ft_philo_create(t_table *table)
{
	int		result;
	int		i;

	table->global->start_time = ft_get_time();
	result = ft_philo_threads(table);
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		table->philo_pid[i] = fork();
		if (table->philo_pid[i] < 0)
			result = ft_err_free_exit("Cannot create philosopher...", table);
		else if (table->philo_pid[i] == 0)
			ft_philo_child(table, i);
		i++;
	}
	pthread_join(table->thr_death, NULL);
	pthread_join(table->thr_repletion, NULL);
	return (result);
}
