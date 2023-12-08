CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
LIBFT = ../../libft/libft.a
INCLUDES = -I../../include -I../../src/tokenizer -I../../libft/include

SRCS = ../../src/tokenizer/reshuffle_quotes.c ../../src/tokenizer/utils.c ../../src/utils/ft_isvalidvarname.c test_reshuffle_quotes.c
OBJS = $(SRCS:.c=.o)
NAME = reshuffle_quotes 


all: $(NAME) $(LIBFT)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
	@$(MAKE) -C ../../libft
	@echo "$(GREEN)Libft compiled$(RESET)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(NAME)

fclean: clean
	rm -f $(NAME)
	rm -f ../../libft/libft.a

re:
	make fclean
	make all

.PHONY: all clean fclean re
