#ifndef CODEXION_H
# define CODEXION_H


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

typedef enum e_sched
{
	FIFO,
	EDF
}	t_sched;

typedef enum e_dongle_state
{
	DONGLE_FREE,
	DONGLE_HELD,
	DONGLE_COOLDOWN
}	t_dongle_state;

typedef struct s_config
{
	int		number_of_coder;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	t_sched	mode;
}	t_config;

typedef struct s_table	t_table;

typedef struct s_dongle
{
	t_dongle_state	state;
	long			released_at;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	void			*waiting;
}	t_dongle;

typedef struct s_coder
{
	int				number;
	long			last_compile_start;
	int				compile_counter;
	int				left_dongle;
	int				right_dongle;
	pthread_mutex_t	compile_lock;
	t_table			*table;
}	t_coder;

typedef struct s_table
{
	t_config		*config;
	long			start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	int				is_over;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	table_lock;
}	t_table;              t_dongle;

t_config parser(int ac, char **av);
void	error_exit(void);
#endif