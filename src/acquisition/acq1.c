/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acq1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:48:55 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 17:50:10 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	am_i_top(t_table *table, t_coder *coder)
{
	if (table->heap.data[0].identity == coder->number)
		return (1);
	else
		return (0);
}
int	is_takeable(t_dongle *d, t_table *table)
{
	if (d->state == DONGLE_FREE || (d->state == DONGLE_COOLDOWN && get_time_ms()
			- d->released_at >= table->config->dongle_cooldown))
		return (1);
	else
		return (0);
}

void	enqueue_coder(t_coder *coder)
{
	t_request	request;

	request.identity = coder->number;
	if (coder->table->config->mode == FIFO)
	{
		request.key = coder->table->arrival_counter;
		coder->table->arrival_counter++;
	}
	else if (coder->table->config->mode == EDF)
	{
		pthread_mutex_lock(&coder->compile_lock);
		request.key = coder->last_compile_start
			+ coder->table->config->time_to_burnout;
		pthread_mutex_unlock(&coder->compile_lock);
	}
	heap_insert(&coder->table->heap, request);
}

void	acquiring(t_coder *coder, t_dongle *left, t_dongle *right)
{
	extract_min(&coder->table->heap);
	left->state = DONGLE_HELD;
	right->state = DONGLE_HELD;
	log_state(coder, "has taken a dongle");
	log_state(coder, "has taken a dongle");
}
void	acquire_pair(t_coder *coder)
{
	struct timespec	deadline;
	t_dongle		*left;
	t_dongle		*right;

	pthread_mutex_lock(&coder->table->table_lock);
	left = &coder->table->dongles[coder->left_dongle];
	right = &coder->table->dongles[coder->right_dongle];
	enqueue_coder(coder);
	while (!(am_i_top(coder->table, coder) && is_takeable(left, coder->table)
			&& is_takeable(right, coder->table) && left != right)
		&& !coder->table->is_over)
	{
		deadline = get_deadline(1);
		pthread_cond_timedwait(&coder->table->table_cond,
			&coder->table->table_lock, &deadline);
	}
	if (coder->table->is_over)
	{
		pthread_mutex_unlock(&coder->table->table_lock);
		return ;
	}
	acquiring(coder, left, right);
	pthread_mutex_unlock(&coder->table->table_lock);
}