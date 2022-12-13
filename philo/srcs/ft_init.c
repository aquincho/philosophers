/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 09:14:44 by aquincho          #+#    #+#             */
/*   Updated: 2022/10/13 09:14:48 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_init_rules(t_table *table, int argc, char **argv)
{
	table->rules = (t_rules *)malloc(sizeof(t_rules));
	if (!table->rules)
		ft_err_free_exit("Cannot initialize table", table);
	table->rules->nbr_philo = ft_atoi_check(argv[1], table);
	table->rules->time_die = ft_atoi_check(argv[2], table);
	table->rules->time_eat = ft_atoi_check(argv[3], table);
	table->rules->time_sleep = ft_atoi_check(argv[4], table);
	if (argc == 6)
		table->rules->nbr_eat = ft_atoi_check(argv[5], table);
	else
		table->rules->nbr_eat = -1;
	if (table->rules->nbr_eat == 0)
		ft_err_free_exit("At least one philosopher has to eat once.", table);
	if (table->rules->nbr_philo == 0)
		ft_err_free_exit("At least one philosopher is needed.", table);
	if (table->rules->time_die < 1 || table->rules->time_eat < 1
		||table->rules->time_sleep < 1)
		ft_err_free_exit("Time must be over 0 ms.", table);
}

static void	ft_init_mutex(t_table *table, int i)
{
	table->mutex_errinit = 0;
	table->mutex_errinit += pthread_mutex_init(&table->msg->m_msg, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_nbr_ate_enough, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_all_ate_enough, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_is_death, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_is_end, NULL);
	while (i < table->rules->nbr_philo)
	{
		table->mutex_errinit
			+= pthread_mutex_init(&table->fork[i].m_fork, NULL);
		table->mutex_errinit
			+= pthread_mutex_init(&table->philo[i].m_time_last_meal, NULL);
		table->mutex_errinit
			+= pthread_mutex_init(&table->philo[i].m_is_dead, NULL);
		table->mutex_errinit
			+= pthread_mutex_init(&table->philo[i].m_eat_count, NULL);
		i++;
	}
	if (table->mutex_errinit != 0)
		ft_err_free_exit("Cannot initialize mutexes", table);
}

static void	ft_init_data_philo(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules->nbr_philo)
	{
		table->fork[i].id = i;
		table->fork[i].is_used = false;
		table->philo[i].id = i;
		table->philo[i].is_dead = false;
		table->philo[i].ate_enough = false;
		table->philo[i].is_end = false;
		table->philo[i].eat_count = 0;
		table->philo[i].time_last_meal = 0;
		table->philo[i].rules = table->rules;
		table->philo[i].lfork = &table->fork[i];
		table->philo[i].rfork = &table->fork[(i + 1) % table->rules->nbr_philo];
		table->philo[i].global = table->global;
		table->philo[i].msg = table->msg;
		i++;
	}
}

static void	ft_init_philos(t_table *table)
{
	table->msg = (t_msg *)malloc(sizeof(*(table->msg)));
	table->global = (t_global *)malloc(sizeof(*(table->global)));
	if (!table->msg || !table->global)
		ft_err_free_exit("Cannot initialize data", table);
	table->global->nbr_ate_enough = 0;
	table->global->all_ate_enough = false;
	table->global->is_death = false;
	table->global->is_end = false;
	table->philo = (t_philo *)malloc
		(sizeof(*(table->philo)) * (table->rules->nbr_philo));
	if (!table->philo)
		ft_err_free_exit("Cannot initialize philosopher", table);
	table->fork
		= (t_fork *)malloc(sizeof(t_fork) * table->rules->nbr_philo);
	if (!table->fork)
		ft_err_free_exit("Cannot initialize fork", table);
	ft_init_data_philo(table);
}

t_table	*ft_init(int argc, char **argv)
{
	t_table	*table;

	table = (t_table *)malloc(sizeof(*table));
	if (!table)
		ft_err_exit("Cannot create rules");
	table->rules = NULL;
	table->philo = NULL;
	table->msg = NULL;
	table->global = NULL;
	table->fork = NULL;
	table->mutex_errinit = -1;
	ft_init_rules(table, argc, argv);
	ft_init_philos(table);
	ft_init_mutex(table, 0);
	return (table);
}
