CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
LIBFT = ../../libft/libft.a
INCLUDES = -I../../include -I../../src/tokenizer -I../../libft/include

SRCS = ../../src/tokenizer/wildcard.c test_wildcard.c ../../src/tokenizer/utils.c
OBJS = $(SRCS:.c=.o)
NAME = test_pattern_builder


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
