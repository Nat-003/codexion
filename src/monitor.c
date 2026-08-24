/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:29:09 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 17:40:29 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	signal_burnout(t_table *table, int i)
{
	pthread_mutex_lock(&table->table_lock);
	table->is_over = 1;
	pthread_cond_broadcast(&table->table_cond);
	pthread_mutex_unlock(&table->table_lock);
	pthread_mutex_lock(&table->print_lock);
	printf("%ld %d burned out\n", (get_time_ms() - table->start_time),
		table->coders[i].number);
	table->print_stop = 1;
	pthread_mutex_unlock(&table->print_lock);
}

int	check_coder_state(t_table *table)
{
	int		i;
	int		number_coders_done;
	long	elapsed;
	int		counter;

	i = 0;
	number_coders_done = 0;
	while (i < table->config->number_of_coder)
	{
		pthread_mutex_lock(&table->coders[i].compile_lock);
		elapsed = get_time_ms() - table->coders[i].last_compile_start;
		counter = table->coders[i].compile_counter;
		pthread_mutex_unlock(&table->coders[i].compile_lock);
		if (elapsed >= table->config->time_to_burnout)
		{
			signal_burnout(table, i);
			break ;
		}
		if (counter >= table->config->number_of_compiles_required)
			number_coders_done++;
		i++;
	}
	return (number_coders_done);
}

void	*monitoring(void *args)
{
	t_table	*table;
	int		coders_done;

	table = (t_table *)args;
	while (!simulation_over(table))
	{
		coders_done = check_coder_state(table);
		if (simulation_over(table))
			break ;
		if (coders_done == table->config->number_of_coder)
		{
			pthread_mutex_lock(&table->table_lock);
			table->is_over = 1;
			pthread_cond_broadcast(&table->table_cond);
			pthread_mutex_unlock(&table->table_lock);
			break ;
		}
		precise_sleep(1);
	}
	return (NULL);
}
