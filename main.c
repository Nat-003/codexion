/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:51:15 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/06 17:00:06 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
		usleep(400);
}




int	main(int ac, char **av)
{
	t_config    config;
	t_table     table;
	memset(&table, 0, sizeof(t_table));
	config = parser(ac, av);
	table.config = &config;
	table.coders = malloc(config.number_of_coder * sizeof(t_coder));
	if (!table.coders)
		error_exit();
	table.dongles = malloc(config.number_of_coder * sizeof(t_dongle));
	if (!table.dongles)
		error_exit_free(table.coders);
    dongle_init(&table);
	coders_init(&table);
	table_mutex_init(&table);
    wire_coders(&table);
    table.start_time = get_time_ms();
    launch_simulation(&table);
    cleanup(&table);
	return (0);
}
