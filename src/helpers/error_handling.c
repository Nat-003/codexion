/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:24:30 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:37:24 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	error_exit(void)
{
	write(2, "Error\n", 6);
	exit(1);
}

void	error_exit_free(void *any)
{
	write(2, "Error\n", 6);
	free(any);
	exit(1);
}

void	cleanup(t_table *table)
{
	int	i;

	if (table->table_cond_initiated)
		pthread_cond_destroy(&table->table_cond);
	if (table->table_lock_initiated)
		pthread_mutex_destroy(&table->table_lock);
	if (table->print_lock_initiated)
		pthread_mutex_destroy(&table->print_lock);
	i = 0;
	while (i < table->coders_initiated)
	{
		pthread_mutex_destroy(&table->coders[i].compile_lock);
		i++;
	}
	i = 0;
	while (i < table->dongles_initiated)
	{
		pthread_mutex_destroy(&table->dongles[i].lock);
		pthread_cond_destroy(&table->dongles[i].cond);
		i++;
	}
	free(table->coders);
	free(table->dongles);
	free(table->heap.data);
}

void	cleanup_and_exit(t_table *table)
{
	cleanup(table);
	error_exit();
}
