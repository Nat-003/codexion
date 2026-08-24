/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_phases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:45:37 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:39:19 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->compile_lock);
	coder->last_compile_start = get_time_ms();
	coder->compile_counter++;
	pthread_mutex_unlock(&coder->compile_lock);
	log_state(coder, "is compiling");
	precise_sleep(coder->table->config->time_to_compile);
}

void	debugging(t_coder *coder)
{
	log_state(coder, "is debugging");
	precise_sleep(coder->table->config->time_to_debug);
}

void	refactor(t_coder *coder)
{
	log_state(coder, "is refactoring");
	precise_sleep(coder->table->config->time_to_refactor);
}
