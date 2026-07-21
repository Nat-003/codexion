#ifndef CODEXION_H
# define CODEXION_H


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
}   t_sched;

typedef struct s_config
{
    int number_of_coder;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    t_sched mode;
}              t_config;


t_config parser(int ac, char **av);
#endif