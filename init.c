/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nappasam <nappasam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 17:26:21 by nappasam          #+#    #+#             */
/*   Updated: 2026/07/27 18:19:44 by nappasam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    dongle_init(t_table *table)
{
    int i;

    i = 0;
    while (i < table->config->number_of_coder)
    {
    if (pthread_mutex_init(&table->dongles[i].lock, NULL) != 0)
        cleanup_and_exit(table);
    if (pthread_cond_init(&table->dongles[i].cond, NULL) != 0)
    {
        pthread_mutex_destroy(&table->dongles[i].lock);
        cleanup_and_exit(table);
    }
    table->dongles_initiated++;
    i++;
    }
}


void    coders_init(t_table *table)
{
    int i;

    i = 0;
    while (i < table->config->number_of_coder)
    {
        if (pthread_mutex_init(&table->coders[i].compile_lock, NULL) != 0)
            cleanup_and_exit(table);
        table->coders_initiated++;
        i++;
    }
    
}

void    table_mutex_init(t_table *table)
{
    if (pthread_mutex_init(&table->print_lock, NULL) != 0)
        cleanup_and_exit(table);
    table->print_lock_initiated = 1;
    if (pthread_mutex_init(&table->table_lock, NULL) != 0)
        cleanup_and_exit(table);
    table->table_lock_initiated = 1;  
}

void    wire_coders(t_table *table)
{
    int i;
    int n;
    
    i = 0;
    n = table->config->number_of_coder;
    while (i < n)
    {
        table->coders[i].number = i + 1;
        table->coders[i].compile_counter = 0;
        table->coders[i].last_compile_start = 0;
        table->coders[i].left_dongle = (i + n - 1) % n;
        table->coders[i].right_dongle = i;
        table->coders[i].table = table;
        i++;
    }
    
}
