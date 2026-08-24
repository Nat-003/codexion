/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 18:59:19 by nappasam          #+#    #+#             */
/*   Updated: 2026/08/24 17:54:47 by nappasam         ###   ########.fr       */
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

void	set_coder_time(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->config->number_of_coder)
	{
		table->coders[i].last_compile_start = get_time_ms();
		i++;
	}
}
