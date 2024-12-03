CC = gcc

NAME = philo

HEADER_NAME = philo.h

FLAGS = -Wall -Wextra -Werror -g -pthread #-fsanitize=address -fsanitize=leak

SRC = 	main.c \
		philo_utils.c \
		create_routine.c \
		routine_utils.c
		
OBJS = $(SRC:.c=.o)

all:
	$(MAKE) $(NAME)
	
$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ 

%.o: %.c Makefile philo.h
	$(CC) $(FLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean r
