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
