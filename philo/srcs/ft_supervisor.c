/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supervisor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 10:11:48 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/18 10:11:50 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_end_super(t_table *table, int i, int type)
{
	int					j;
	unsigned long long	time;

	time = ft_get_time();
	j = 0;
	while (j < table->rules->nbr_philo)
	{
		pthread_join(table->philo[j].thr_id, NULL);
		j++;
	}
	ft_display_msg(&table->philo[i], time, type);
}

static bool	ft_end_event(t_table *table, int type)
{
	if (type == TYPE_DIE)
		table->msg->type = TYPE_DIE;
	if (type == TYPE_ENOUGH)
	{
		table->global->all_ate_enough = true;
		table->msg->type = TYPE_ENOUGH;
	}
	return (true);
}

static void	ft_check_repletion(t_table *table, int i)
{
	pthread_mutex_lock(&table->philo[i].m_eat_count);
	if (table->philo[i].rules->nbr_eat > 0
		&& table->philo[i].eat_count >= table->philo[i].rules->nbr_eat
		&& !table->philo[i].is_dead && !table->philo[i].ate_enough)
	{
		table->philo[i].ate_enough = true;
		pthread_mutex_lock(&table->global->m_nbr_ate_enough);
		table->global->nbr_ate_enough++;
		pthread_mutex_unlock(&table->global->m_nbr_ate_enough);
	}
	pthread_mutex_unlock(&table->philo[i].m_eat_count);
	if ((table->global->nbr_ate_enough == table->rules->nbr_philo
			|| table->global->all_ate_enough)
		&& !table->global->is_death)
	{
		pthread_mutex_lock(&table->global->m_is_end);
		table->global->is_end = ft_end_event(table, TYPE_ENOUGH);
		pthread_mutex_unlock(&table->global->m_is_end);
	}
}

static int	ft_test_end(t_table *table, int i)
{
	pthread_mutex_lock(&table->philo[i].m_time_last_meal);
	if (table->philo[i].time_last_meal > 0)
	{
		pthread_mutex_unlock(&table->philo[i].m_time_last_meal);
		pthread_mutex_lock(&table->philo[i].m_is_dead);
		table->philo[i].is_dead = ft_isdead(&table->philo[i]);
		pthread_mutex_unlock(&table->philo[i].m_is_dead);
		pthread_mutex_lock(&table->philo[i].m_time_last_meal);
	}
	pthread_mutex_unlock(&table->philo[i].m_time_last_meal);
	if (table->philo[i].is_dead || table->global->is_death)
	{
		pthread_mutex_lock(&table->global->m_is_end);
		table->global->is_end = ft_end_event(table, TYPE_DIE);
		pthread_mutex_unlock(&table->global->m_is_end);
	}
	ft_check_repletion(table, i);
	usleep(5);
	return (i + 1);
}

void	*ft_supervisor(void *data)
{
	t_table	*table;
	int		i;
	int		result;

	table = (t_table *)data;
	result = ft_philo_create(table);
	if (result)
	{
		result = ft_err_free_exit("Cannot create philosophers.", (void *)table);
		return (NULL);
	}
	while (!table->global->is_end)
	{
		i = 0;
		while (i < table->rules->nbr_philo && !table->global->is_end)
			i = ft_test_end(table, i);
	}
	ft_end_super(table, i - 1, table->msg->type);
	return (NULL);
}
