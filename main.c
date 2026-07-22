#include "codexion.h"


#include "codexion.h"

static const char	*mode_str(t_sched mode)
{
	if (mode == FIFO)
		return ("fifo");
	return ("edf");
}
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
		usleep(400);
}


void	error_exit_free(void *any)
{
	write(2, "Error\n", 6);
    free(any);
	exit(1);
}

int	main(int ac, char **av)
{
	t_config	config;
    t_table		table;

    memset(&table, 0, sizeof(t_table));
    config = parser(ac, av);
    table.config = &config;
    table.coders = malloc(config.number_of_coder * sizeof(t_coder));
    if (!table.coders)
        error_exit();
    table.dongles = malloc(config.number_of_coder * sizeof(t_dongle));
    if (!table.dongles)
        error_exit_free(table.coders);
	// printf("--- parse OK ---\n");
	// printf("number_of_coder            = %d\n", config.number_of_coder);
	// printf("time_to_burnout            = %d\n", config.time_to_burnout);
	// printf("time_to_compile            = %d\n", config.time_to_compile);
	// printf("time_to_debug              = %d\n", config.time_to_debug);
	// printf("time_to_refactor           = %d\n", config.time_to_refactor);
	// printf("number_of_compiles_required = %d\n",
	// 	config.number_of_compiles_required);
	// printf("dongle_cooldown            = %d\n", config.dongle_cooldown);
	// printf("mode                       = %s\n", mode_str(config.mode));
    // printf("time testing \n");
    // struct timeval	tv;

	// gettimeofday(&tv, NULL);
	// printf("seconds:      %ld\n", tv.tv_sec);
	// printf("microseconds: %ld\n", tv.tv_usec);
    // long start = get_time_ms();
    // printf("%ld\n",start);
    // precise_sleep(200);
    // long end = get_time_ms();
    // long elapsed = end - start;
    // printf("%ld\n", elapsed);

	return (0);
}