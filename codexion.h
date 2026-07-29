/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:51:21 by nappasam          #+#    #+#             */
/*   Updated: 2026/07/29 17:14:59 by nappasam         ###   ########.fr       */
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
    pthread_t		    thread;  
}						t_coder;

typedef struct s_table
{
	t_config			*config;
	long				start_time;
	t_coder				*coders;
	t_dongle			*dongles;
	int					is_over;
	pthread_mutex_t		print_lock;
	pthread_mutex_t		table_lock;
    pthread_cond_t      table_cond;
	int					dongles_initiated;
    int                 coders_initiated;
    int                 print_lock_initiated;
    int                 table_lock_initiated;
    int                 table_cond_initiated;
    
}						t_table;

t_config				parser(int ac, char **av);
void					error_exit(void);
void	                error_exit_free(void *any);
void	                cleanup(t_table *table);
void	                cleanup_and_exit(t_table *table);
void                    dongle_init(t_table *table);
void                    coders_init(t_table *table);
void                    table_mutex_init(t_table *table);
void                    wire_coders(t_table *table);
long	                get_time_ms(void);
void	                precise_sleep(long ms);
void                    *coder_routine(void *arg);
int	                    launch_simulation(t_table *table);
#endif