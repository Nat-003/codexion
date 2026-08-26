/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acq2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:51:15 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:43:00 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_pair(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		now;

	pthread_mutex_lock(&coder->table->table_lock);
	now = get_time_ms();
	left = &coder->table->dongles[coder->left_dongle];
	right = &coder->table->dongles[coder->right_dongle];
	left->state = DONGLE_COOLDOWN;
	left->released_at = now;
	right->state = DONGLE_COOLDOWN;
	right->released_at = now;
	pthread_cond_broadcast(&coder->table->table_cond);
	pthread_mutex_unlock(&coder->table->table_lock);
}

int	i_win_dongle(t_coder *coder, int d)
{
	int			n;
	int			od;
	t_coder		*other;
	t_request	mine;
	t_request	theirs;

	n = coder->table->config->number_of_coder;
	if (d == coder->number - 1)
		other = &coder->table->coders[(d + 1) % n];
	else
		other = &coder->table->coders[d];
	if (!other->is_waiting)
		return (1);
	if (d == other->left_dongle)
		od = other->right_dongle;
	else
		od = other->left_dongle;
	if (!is_takeable(&coder->table->dongles[od], coder->table))
		return (1);
	mine = make_request(coder);
	theirs = make_request(other);
	return (is_before(mine, theirs));
}

t_request	make_request(t_coder *coder)
{
	t_request	r;

	r.identity = coder->number;
	if (coder->table->config->mode == FIFO)
		r.key = coder->arrival_key;
	else
	{
		pthread_mutex_lock(&coder->compile_lock);
		r.key = coder->last_compile_start
			+ coder->table->config->time_to_burnout;
		pthread_mutex_unlock(&coder->compile_lock);
	}
	return (r);
}