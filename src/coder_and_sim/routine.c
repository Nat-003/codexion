/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:52:57 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:39:03 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_over(t_table *table)
{
	int	over;

	pthread_mutex_lock(&table->table_lock);
	over = table->is_over;
	pthread_mutex_unlock(&table->table_lock);
	return (over);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!simulation_over(coder->table))
	{
		acquire_pair(coder);
		if (simulation_over(coder->table))
			break ;
		compiling(coder);
		release_pair(coder);
		debugging(coder);
		refactor(coder);
	}
	return (NULL);
}

int	launch_simulation(t_table *table)
{
	int	i;

	table->start_time = get_time_ms();
	i = 0;
	set_coder_time(table);
	while (i < table->config->number_of_coder)
	{
		if (pthread_create(&table->coders[i].thread, NULL, coder_routine,
				&table->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&table->monitor, NULL, monitoring, table))
		return (1);
	i = 0;
	while (i < table->config->number_of_coder)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
	pthread_join(table->monitor, NULL);
	return (0);
}
