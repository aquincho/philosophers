/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aquincho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 12:00:00 by aquincho          #+#    #+#             */
/*   Updated: 2022/11/03 14:25:00 by aquincho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		result = ft_philo_create(table);
	ft_free_table(table);
	return (result);
}
