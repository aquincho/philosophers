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

#include "philo_bonus.h"

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
		ft_err_free_exit("Philosophers must eat at least once.", table);
	if (table->rules->nbr_philo == 0)
		ft_err_free_exit("At least one philosopher is needed.", table);
	if (table->rules->time_die < 1 || table->rules->time_eat < 1
		||table->rules->time_sleep < 1)
		ft_err_free_exit("Time must be over 0 ms.", table);
}

static void	ft_init_mutex(t_table *table)
{
	table->mutex_errinit = 0;
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_is_death, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->global->m_repletion, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->philo->m_is_dead, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->philo->m_time_last_meal, NULL);
	table->mutex_errinit
		+= pthread_mutex_init(&table->philo->m_eat_count, NULL);
	if (table->mutex_errinit != 0)
		ft_err_free_exit("Cannot initialize mutexes", table);
}

static void	ft_init_semaphore(t_table *table)
{
	sem_unlink("/pmsg");
	sem_unlink("/ptake");
	sem_unlink("/pfork");
	sem_unlink("/prepletion");
	sem_unlink("/pdeath");
	table->msg->sem_msg = sem_open("/pmsg", O_CREAT, 0644, 1);
	table->global->sem_take_forks = sem_open("/ptake", O_CREAT, 0644, 1);
	table->sem_fork = sem_open("/pfork", O_CREAT, 0644,
			table->rules->nbr_philo);
	table->global->sem_repletion = sem_open("/prepletion", O_CREAT, 0644, 1);
	table->global->sem_is_death = sem_open("/pdeath", O_CREAT, 0644, 1);
	if (table->msg->sem_msg == SEM_FAILED
		|| table->global->sem_take_forks == SEM_FAILED
		|| table->sem_fork == SEM_FAILED
		|| table->global->sem_repletion == SEM_FAILED
		|| table->global->sem_is_death == SEM_FAILED)
	{
		table->semaphore_errinit = 1;
		ft_err_free_exit("Cannot initialize semaphores!", table);
	}
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
	table->semaphore_errinit = -1;
	ft_init_rules(table, argc, argv);
	ft_init_philos(table);
	ft_init_semaphore(table);
	ft_init_mutex(table);
	ft_init_data_philo(table, 0);
	return (table);
}
