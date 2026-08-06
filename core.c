/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 18:59:19 by nappasam          #+#    #+#             */
/*   Updated: 2026/07/30 22:33:30 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void log_state(t_coder *coder, char *message)
{
    pthread_mutex_lock(&coder->table->print_lock);
    if (coder->table->print_stop)
    {
        pthread_mutex_unlock(&coder->table->print_lock);
        return ;
    }
    else
    {
        printf("%ld %d %s\n",(get_time_ms() - coder->table->start_time), coder->number ,message);
        pthread_mutex_unlock(&coder->table->print_lock);
    }
    
}
void log_burnout(t_coder *coder, char *message)
{

    pthread_mutex_lock(&coder->table->print_lock);
    printf("%ld %d %s\n",(get_time_ms() - coder->table->start_time), coder->number ,message);
    pthread_mutex_unlock(&coder->table->print_lock);

}

void    compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->compile_lock);
    coder->last_compile_start = get_time_ms();
    coder->compile_counter++;
    pthread_mutex_unlock(&coder->compile_lock);
    log_state(coder,"is compiling");
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
int is_takeable(t_dongle *d, t_table *table)
{
    if (d->state == DONGLE_FREE || (d->state == DONGLE_COOLDOWN && get_time_ms() - d->released_at >= table->config->dongle_cooldown))
        return 1;
    else
        return 0;
}
struct timespec	get_deadline(long ms)
{
	struct timespec	ts;
	long			extra_ns;

	clock_gettime(CLOCK_REALTIME, &ts);
	extra_ns = (ms % 1000) * 1000000L;
	ts.tv_sec += ms / 1000;
	ts.tv_nsec += extra_ns;
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_nsec -= 1000000000L;
		ts.tv_sec += 1;
	}
	return (ts);
}
void acquire_pair(t_coder *coder)
{
    t_dongle	    *left;
    t_dongle	    *right;
    struct timespec	deadline;
    pthread_mutex_lock(&coder->table->table_lock);
    left = &coder->table->dongles[coder->left_dongle];
    right = &coder->table->dongles[coder->right_dongle];    
    while (((!is_takeable(left, coder->table) || !is_takeable(right, coder->table))
        || left == right)
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
    long        now;
    
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

int simulation_over(t_table *table)
{
    int over;

    pthread_mutex_lock(&table->table_lock);
    over = table->is_over;
    pthread_mutex_unlock(&table->table_lock);
    return over;
}

void *coder_routine(void *arg)
{
    t_coder *coder;

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
    while (i < table->config->number_of_coder)
    {
        table->coders[i].last_compile_start = get_time_ms();
        i++;
    }
    i = 0;
	while (i < table->config->number_of_coder)
	{
		if (pthread_create(&table->coders[i].thread, NULL,
				coder_routine, &table->coders[i]) != 0)
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
