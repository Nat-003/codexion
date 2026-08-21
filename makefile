

NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= main.c \
			  parser.c \
			  error_handling.c \
			  init.c \
			  core.c \
			  monitor.c \
			  heap.c

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