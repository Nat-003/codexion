/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:51:21 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:01:48 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_sched
{
	FIFO,
	EDF
}						t_sched;

typedef enum e_dongle_state
{
	DONGLE_FREE,
	DONGLE_HELD,
	DONGLE_COOLDOWN
}						t_dongle_state;

typedef struct s_config
{
	int					number_of_coder;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	t_sched				mode;
}						t_config;

typedef struct s_table	t_table;
typedef struct s_heap	t_heap;
typedef struct s_dongle
{
	t_dongle_state		state;
	long				released_at;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	void				*waiting;
}						t_dongle;

typedef struct s_coder
{
	int					number;
	long				last_compile_start;
	int					compile_counter;
	int					left_dongle;
	int					right_dongle;
	pthread_mutex_t		compile_lock;
	t_table				*table;
	pthread_t			thread;
}						t_coder;

typedef struct s_request
{
	int					identity;
	long				key;
}						t_request;

typedef struct s_heap
{
	t_request			*data;
	int					size;
	int					capacity;
}						t_heap;

typedef struct s_table
{
	t_config			*config;
	long				start_time;
	t_coder				*coders;
	t_dongle			*dongles;
	int					is_over;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		table_lock;
	pthread_cond_t		table_cond;
	int					dongles_initiated;
	int					coders_initiated;
	int					print_lock_initiated;
	int					table_lock_initiated;
	int					table_cond_initiated;
	int					print_stop;
	pthread_t			monitor;
	t_heap				heap;
	long				arrival_counter;

}						t_table;

t_config				parser(int ac, char **av);
void					error_exit(void);
void					error_exit_free(void *any);
void					cleanup(t_table *table);
void					cleanup_and_exit(t_table *table);
void					dongle_init(t_table *table);
void					coders_init(t_table *table);
void					table_mutex_init(t_table *table);
void					wire_coders(t_table *table);
long					get_time_ms(void);
void					precise_sleep(long ms);
int						launch_simulation(t_table *table);
void					log_state(t_coder *coder, char *message);
void					*monitoring(void *args);
int						simulation_over(t_table *table);
void					log_burnout(t_coder *coder, char *message);
int						init_heap(t_heap *heap, int capacity);
void					heap_insert(t_heap *heap, t_request value);
t_request				extract_min(t_heap *heap);
void	                compiling(t_coder *coder);
void	                debugging(t_coder *coder);
void	                refactor(t_coder *coder);
int	                    am_i_top(t_table *table, t_coder *coder);
int	                    is_takeable(t_dongle *d, t_table *table);
void	                enqueue_coder(t_coder *coder);
void	                acquiring(t_coder *coder, t_dongle *left, t_dongle *right);
void	                acquire_pair(t_coder *coder);
void	                release_pair(t_coder *coder);
void	                *coder_routine(void *arg);
void	                set_coder_time(t_table *table);
struct timespec	        get_deadline(long ms);
void	                swap(t_request *a, t_request *b);
#endif