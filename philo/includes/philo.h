/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 08:57:52 by aquincho          #+#    #+#             */
/*   Updated: 2022/09/23 08:59:13 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>

typedef enum e_msg_type
{
	TYPE_FORK,
	TYPE_EAT,
	TYPE_SLEEP,
	TYPE_THINK,
	TYPE_DIE,
	TYPE_ENOUGH
}	t_msg_type;

typedef struct s_rules
{
	int				nbr_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nbr_eat;
}	t_rules;

typedef struct s_fork
{
	int				id;
	bool			is_used;
	pthread_mutex_t	m_fork;
}	t_fork;

typedef struct s_msg
{
	int				type;
	pthread_mutex_t	m_msg;
}	t_msg;

typedef struct s_global
{
	bool				is_end;
	bool				is_death;
	bool				all_ate_enough;
	int					nbr_ate_enough;
	unsigned long long	start_time;
	pthread_mutex_t		m_nbr_ate_enough;
	pthread_mutex_t		m_all_ate_enough;
	pthread_mutex_t		m_is_death;
	pthread_mutex_t		m_is_end;
}	t_global;

typedef struct s_philo
{
	int					id;
	pthread_t			thr_id;
	bool				is_dead;
	bool				ate_enough;
	bool				is_end;
	int					eat_count;
	unsigned long long	time_last_meal;
	t_fork				*lfork;
	t_fork				*rfork;
	t_rules				*rules;
	t_global			*global;
	t_msg				*msg;
	pthread_mutex_t		m_time_last_meal;
	pthread_mutex_t		m_is_end;
	pthread_mutex_t		m_is_dead;
	pthread_mutex_t		m_eat_count;
}	t_philo;

typedef struct s_table
{
	t_rules				*rules;
	t_philo				*philo;
	t_fork				*fork;
	t_msg				*msg;
	t_global			*global;
	pthread_t			thr_superv;
	int					mutex_errinit;
}	t_table;

/* Rules initialization ft_init.c */
t_table				*ft_init(int argc, char **argv);
/* Thread supervisor ft_supervisor.c*/
void				*ft_supervisor(void *data);
/* Philo thread create and manage ft_philo_life.c */
int					ft_philo_create(t_table *table);
bool				ft_philo_end(t_philo *philo);
/* Fork management ft_forks.c */
bool				ft_take_forks(t_philo *philo);
void				ft_release_forks(t_philo *philo, bool left, bool right);
/* Actons management ft_actions.c */
bool				ft_eat(t_philo *philo);
bool				ft_sleep(t_philo *philo);
bool				ft_think(t_philo *philo, int time, bool start_even);
/* Messages display */
void				ft_display_msg(t_philo *philo, unsigned long long time,
						int type);
/* Time management ft_time.c */
unsigned long long	ft_get_time(void);
int					ft_time_left(t_philo *philo);
bool				ft_isdead(t_philo *philo);
/* Errors management ft_errors.c */
int					ft_strlen(const char *s);
int					ft_err_exit(char *msg);
int					ft_err_free_exit(char *msg, t_table *table);
/* Memory free ft_free.c */
void				ft_free_table(t_table *table);
/* Utilitaries ft_utils.c */
int					ft_strlen(const char *s);
int					ft_atoi_check(const char *nptr, t_table *table);

#endif
