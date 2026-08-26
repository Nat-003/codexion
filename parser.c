/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:51:06 by nappasam          #+#    #+#             */
/*   Updated: 2026/07/27 16:24:18 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



int	is_valid_num(char *s)
{
	int		i;
	long	res;

	res = 0;
	i = 0;
	if (s[i] == '+')
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		res = res * 10 + (s[i] - '0');
		if (res > INT_MAX)
			return (0);
		i++;
	}
	return (1);
}

void	assign_value(char **av, t_config *config)
{
	config->number_of_coder = atoi(av[1]);
	config->time_to_burnout = atoi(av[2]);
	config->time_to_compile = atoi(av[3]);
	config->time_to_debug = atoi(av[4]);
	config->time_to_refactor = atoi(av[5]);
	config->number_of_compiles_required = atoi(av[6]);
	config->dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		config->mode = FIFO;
	else if (strcmp(av[8], "edf") == 0)
		config->mode = EDF;
	else
		error_exit();
	return ;
}

int	validate_range(t_config *config)
{
	if (config->number_of_coder == 0 || config->time_to_burnout == 0
		|| config->time_to_compile == 0
		|| config->number_of_compiles_required == 0)
		return (0);
	return (1);
}

t_config	parser(int ac, char **av)
{
	int			i;
	t_config	config;

	i = 1;
	memset(&config, 0, sizeof(t_config));
	if (ac != 9)
		error_exit();
	while (i < ac - 1)
	{
		if (!is_valid_num(av[i]))
			error_exit();
		i++;
	}
	assign_value(av, &config);
	if (!validate_range(&config))
		error_exit();
	return (config);
}
