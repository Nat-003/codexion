/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:51:15 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 18:32:05 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	dongle_init(&table);
	coders_init(&table);
	table_mutex_init(&table);
	wire_coders(&table);
	if (init_heap(&table.heap, config.number_of_coder))
		cleanup_and_exit(&table);
	table.start_time = get_time_ms();
	launch_simulation(&table);
	cleanup(&table);
	return (0);
}
