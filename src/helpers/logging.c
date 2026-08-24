/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:43:23 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:00:24 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->table->print_lock);
	if (coder->table->print_stop)
	{
		pthread_mutex_unlock(&coder->table->print_lock);
		return ;
	}
	else
	{
		printf("%ld %d %s\n", (get_time_ms() - coder->table->start_time),
			coder->number, message);
		pthread_mutex_unlock(&coder->table->print_lock);
	}
}
void	log_burnout(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->table->print_lock);
	printf("%ld %d %s\n", (get_time_ms() - coder->table->start_time),
		coder->number, message);
	pthread_mutex_unlock(&coder->table->print_lock);
}
