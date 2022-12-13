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
	philo->time_last_meal = table->global->start_time;
	philo_end = (philo->is_dead || philo->ate_enough);
	sem_wait(table->global->m_take_forks);
	while (!philo_end)
	{
		philo_end = ft_take_forks(table);
		if (!philo_end)
			philo_end = ft_eat(table);
		if (!philo_end)
			philo_end = ft_sleep(table);
	}
	return (philo->is_dead);
}

static void	ft_philo_child(t_table *table, int i)
{
	int	result;

	ft_init_data_philo(table, i);
	table->philo->id = i;
	table->philo->time_last_meal = table->global->start_time;
	result = pthread_create(&table->philo->thr_superv, NULL,
			&ft_philo_supervisor, (void *)(table));
	if (result)
		ft_err_free_exit("Cannot supervize philosopher...", table);
	result = pthread_create(&table->philo->thr_end_philo, NULL,
			&ft_end_philo_supervisor, (void *)(table));
	if (result)
		ft_err_free_exit("Cannot supervize death philosopher...", table);
	result = ft_philo_life(table);
	pthread_join(table->philo->thr_end_philo, NULL);
	pthread_join(table->philo->thr_superv, NULL);
	ft_free_table (table);
	exit (0);
}

static int	ft_philo_threads(t_table *table)
{
	int	result;

	result = 0;
	sem_wait(table->global->m_is_death);
	sem_wait(table->global->m_is_end);
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

static void	ft_philo_wait(t_table *table, int *result)
{
	int	i;
	int	res_pid;

	i = -1;
	while (++i < table->rules->nbr_philo)
		sem_post(table->msg->m_msg);
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		waitpid(-1, &res_pid, 0);
		if (res_pid == 512)
			(*result)++;
		i++;
	}
}

int	ft_philo_create(t_table *table)
{
	int		result;
	int		i;
	pid_t	pid;

	table->global->start_time = ft_get_time();
	result = ft_philo_threads(table);
	i = 0;
	while (i < table->rules->nbr_philo)
	{
		pid = fork();
		if (pid < 0)
			result = ft_err_free_exit("Cannot create philosopher...", table);
		else if (pid == 0)
			ft_philo_child(table, i);
		i++;
	}
	pthread_join(table->thr_death, NULL);
	pthread_join(table->thr_repletion, NULL);
	usleep(1000);
	ft_philo_wait(table, &result);
	return (result);
}
