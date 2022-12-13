/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 08:57:37 by aquincho          #+#    #+#             */
/*   Updated: 2022/09/23 08:57:40 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int			result;
	t_table		*table;

	result = 0;
	if (argc < 5 || argc > 6)
		result = ft_err_exit("Wrong number of arguments.");
	table = ft_init(argc, argv);
	if (!table)
		result = ft_err_exit("Cannot create table.");
	if (!result)
		result = pthread_create(&table->thr_superv, NULL, &ft_supervisor,
				table);
	if (result)
	{
		result = ft_err_free_exit("Cannot create philosophers.", (void *)table);
		return (result);
	}
	pthread_join(table->thr_superv, NULL);
	ft_free_table(table);
	return (result);
}
