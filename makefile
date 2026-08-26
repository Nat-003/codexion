NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -I.

SRCS		= main.c \
			  src/parser.c \
			  src/monitor.c \
			  src/acquisition/acq1.c \
			  src/acquisition/acq2.c \
			  src/binary_heap/heap.c \
			  src/binary_heap/heap2.c \
			  src/coder_and_sim/coder_phases.c \
			  src/coder_and_sim/init.c \
			  src/coder_and_sim/routine.c \
			  src/helpers/error_handling.c \
			  src/helpers/time.c \
			  src/helpers/logging.c

OBJS		= $(SRCS:.c=.o)

HEADER		= codexion.h

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:		%.c $(HEADER)
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re