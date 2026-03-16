CC		= cc
CFLAGS	= -Wall -Wextra -Werror

NAME	= sandbox.a
SRCS	= sandbox.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

test: $(NAME)
	$(CC) $(CFLAGS) tester.c $(NAME) -o tester
	./tester

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) tester

re: fclean all

.PHONY: all clean fclean re test