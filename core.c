/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 18:59:19 by nappasam          #+#    #+#             */
/*   Updated: 2026/07/30 19:05:12 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void log_state(t_coder *coder, char *message)
{
    pthread_mutex_lock(&coder->table->print_lock);
    printf("%ld %d %s\n",(get_time_ms() - coder->table->start_time), coder->number ,message);
    pthread_mutex_unlock(&coder->table->print_lock);
}

void    compiling(t_coder *coder)
{
    coder->last_compile_start = get_time_ms();
    log_state(coder,"is compiling");
    coder->compile_counter++;
    precise_sleep(coder->table->config->time_to_compile);
}

void    debugging(t_coder *coder)
{
    log_state(coder,"is debugging");
    precise_sleep(coder->table->config->time_to_debug);
}

void    refactor(t_coder *coder)
{
    log_state(coder,"is refactoring");
    precise_sleep(coder->table->config->time_to_refactor);
}

void acquire_pair(t_coder *coder)
{
    t_dongle	*left;
    t_dongle	*right;

    pthread_mutex_lock(&coder->table->table_lock);
    left = &coder->table->dongles[coder->left_dongle];
    right = &coder->table->dongles[coder->right_dongle];
    while (left->state != DONGLE_FREE || right->state != DONGLE_FREE)
    {
        pthread_cond_wait(&coder->table->table_cond, &coder->table->table_lock);
    }
    left->state = DONGLE_HELD;
    right->state = DONGLE_HELD;
    log_state(coder, "has taken a dongle");
    log_state(coder, "has taken a dongle");
    pthread_mutex_unlock(&coder->table->table_lock);
}

void release_pair(t_coder *coder)
{
    t_dongle	*left;
    t_dongle	*right;
    
    pthread_mutex_lock(&coder->table->table_lock);
    left = &coder->table->dongles[coder->left_dongle];
    right = &coder->table->dongles[coder->right_dongle];
    left->state = DONGLE_FREE;
    right->state = DONGLE_FREE;
    pthread_cond_broadcast(&coder->table->table_cond);
    pthread_mutex_unlock(&coder->table->table_lock);
}

void *coder_routine(void *arg)
{
    t_coder *coder;
    int iteration;

    coder = (t_coder *)arg;
    iteration = 0;
    coder->last_compile_start = get_time_ms();
    while (iteration < 20)
    {
        acquire_pair(coder);
        compiling(coder);
        release_pair(coder);
        debugging(coder);
        refactor(coder);
        iteration++;
    }
    return (NULL);
}

int	launch_simulation(t_table *table)
{
	int	i;

	table->start_time = get_time_ms();
	i = 0;
	while (i < table->config->number_of_coder)
	{
		if (pthread_create(&table->coders[i].thread, NULL,
				coder_routine, &table->coders[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < table->config->number_of_coder)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
	return (0);
}