#include "codexion.h"


#include "codexion.h"

static const char	*mode_str(t_sched mode)
{
	if (mode == FIFO)
		return ("fifo");
	return ("edf");
}

int	main(int ac, char **av)
{
	t_config	config;

	config = parser(ac, av);
	printf("--- parse OK ---\n");
	printf("number_of_coder            = %d\n", config.number_of_coder);
	printf("time_to_burnout            = %d\n", config.time_to_burnout);
	printf("time_to_compile            = %d\n", config.time_to_compile);
	printf("time_to_debug              = %d\n", config.time_to_debug);
	printf("time_to_refactor           = %d\n", config.time_to_refactor);
	printf("number_of_compiles_required = %d\n",
		config.number_of_compiles_required);
	printf("dongle_cooldown            = %d\n", config.dongle_cooldown);
	printf("mode                       = %s\n", mode_str(config.mode));
	return (0);
}