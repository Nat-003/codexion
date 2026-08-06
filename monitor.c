#include "codexion.h"

void *monitoring(void *args)
{
    t_table *table;
    int i;
    int number_coders_done;
    long elapsed;
    int counter;
    table = (t_table *)args;
    while (!simulation_over(table))
    {
        i = 0;
        number_coders_done = 0;
        while (i < table->config->number_of_coder)
        {
            pthread_mutex_lock(&table->coders[i].compile_lock);
            elapsed = get_time_ms() - table->coders[i].last_compile_start;
            counter = table->coders[i].compile_counter;
            pthread_mutex_unlock(&table->coders[i].compile_lock);
            if (elapsed >= table->config->time_to_burnout)
            {
                pthread_mutex_lock(&table->table_lock);
                table->is_over = 1;
                pthread_cond_broadcast(&table->table_cond);
                pthread_mutex_unlock(&table->table_lock);
                pthread_mutex_lock(&table->print_lock);
                printf("%ld %d burned out\n",(get_time_ms() - table->start_time), table->coders[i].number);
                table->print_stop = 1;
                pthread_mutex_unlock(&table->print_lock);
                break ;
            }
            if (counter >= table->config->number_of_compiles_required)
                number_coders_done++;
            i++;         
        }
        if (number_coders_done == table->config->number_of_coder)
        {
            pthread_mutex_lock(&table->table_lock);
            table->is_over = 1;
            pthread_cond_broadcast(&table->table_cond);
            pthread_mutex_unlock(&table->table_lock);
            break;
        }
        precise_sleep(1);
    }
    return (NULL);
}
