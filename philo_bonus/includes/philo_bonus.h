/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 12:00:00 by aquincho          #+#    #+#             */
/*   Updated: 2022/11/03 14:25:00 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <semaphore.h>

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

typedef struct s_msg
{
	char	*msg;
	int		type;
	sem_t	*m_msg;
}	t_msg;

typedef struct s_global
{
	bool				is_death;
	bool				repletion;
	unsigned long long	start_time;
	unsigned long long	end_time;
	sem_t				*m_take_forks;
	sem_t				*m_repletion;
	sem_t				*m_is_death;
	sem_t				*m_is_end;
}	t_global;

typedef struct s_philo
{
	int					id;
	pthread_t			thr_superv;
	pthread_t			thr_end_philo;
	bool				is_dead;
	bool				is_end;
	bool				ate_enough;
	int					eat_count;
	unsigned long long	time_last_meal;
	int					time_left;
}	t_philo;

typedef struct s_table
{
	t_rules		*rules;
	t_philo		*philo;
	sem_t		*m_fork;
	t_msg		*msg;
	t_global	*global;
	pthread_t	thr_repletion;
	pthread_t	thr_death;
	int			semaphore_errinit;
}	t_table;

/* Rules initialization ft_init.c */
t_table				*ft_init(int argc, char **argv);
void				ft_init_data_philo(t_table *table, int i);
/* Thread supervisor ft_supervisor.c*/
void				*ft_death_supervisor(void *data);
void				*ft_repletion_supervisor(void *data);
void				*ft_philo_supervisor(void *data);
void				*ft_end_philo_supervisor(void *data);
/* Philo thread create and manage ft_philo_life.c */
int					ft_philo_create(t_table *table);
/* Fork management ft_actions.c */
bool				ft_take_forks(t_table *table);
bool				ft_eat(t_table *table);
bool				ft_sleep(t_table *table);
/* Messages display */
void				ft_display_msg(t_table *table, unsigned long long time,
						int type);
/* Time management ft_time.c */
unsigned long long	ft_get_time(void);
int					ft_time_left(t_table *table);
bool				ft_isdead(t_table *table);
/* Errors management ft_errors.c */
int					ft_err_exit(char *msg);
int					ft_err_free_exit(char *msg, t_table *table);
int					ft_err_free_exit_philo(char *msg, t_philo *philo);
/* Memory free ft_free.c */
void				ft_free_table(t_table *table);
/* Utilitaries ft_utils.c */
int					ft_strlen(const char *s);
int					ft_atoi_check(const char *nptr, t_table *table);
void				*ft_memcpy(void *dest, const void *src, size_t n);

#endif
