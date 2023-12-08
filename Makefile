CC = gcc

NAME =	minishell 
CFLAGS = -g -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
LIBS = -lreadline
INCLUDES = -I./include  -I./libft/include -I./src/tokenizer
SRC_DIR = src
OBJ_DIR = obj

UNAME_S := $(shell uname -s)

# # Set include paths conditionally
ifeq ($(UNAME_S), Darwin)
    INCLUDES = -I./include -I./libft/include -I/opt/homebrew/opt/readline/include -I./libft/include -I/usr/local/opt/readline/include -I./src/tokenizer

else
    INCLUDES = -I./include -I./libft/include -I./src/tokenizer
endif

ifeq ($(UNAME_S), Darwin)
    LDFLAGS = -L/opt/homebrew/opt/readline/lib -L/usr/local/opt/readline/lib
else
    LDFLAGS = 
endif

# For batu's mac
# ifeq ($(UNAME_S), Darwin)
#     INCLUDES = -I./include -I./libft/include -I/usr/local/opt/readline/include
# else
#     INCLUDES = -I./include -I./libft/include
# endif

# ifeq ($(UNAME_S), Darwin)
#     LDFLAGS = -L/usr/local/opt/readline/lib
# else
#     LDFLAGS = 
# endif

SRCS = minishell.c \
 tokenizer/tokenizer.c tokenizer/utils.c tokenizer/assign.c tokenizer/count_words.c tokenizer/assign_redirect.c tokenizer/wildcard.c \
 lexer/lexer.c lexer/lexemes_redirect.c lexer/lexemes.c lexer/quotes.c lexer/var_subs.c lexer/wrappers.c utils/utils.c \
 parser/parser.c parser/create_node.c parser/fill_node_cmd_args.c parser/fill_node_redirects.c parser/parser_utils.c \
  utils/check_input.c utils/free_functions.c utils/init.c \
 builtins/pwd.c builtins/env.c builtins/export.c builtins/unset.c builtins/cd.c builtins/exit.c builtins/echo.c utils/read_input.c  \
 executor/redirections.c executor/heredoc.c executor/execute_builtins.c executor/executor.c executor/utils.c executor/handle_pipes.c \
 env_vars.c utils/ft_isvalidvarname.c tokenizer/reshuffle_quotes.c \
 utils/signals.c executor/wait.c debug/debug.c  tokenizer/reshuffle_quotes_utils.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS := $(OBJS:/=_)

all: $(NAME) $(LIBFT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)  
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@  


$(NAME): $(OBJS) $(LIBFT) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)  $(LDFLAGS)

$(LIBFT):
	@$(MAKE) -C ./libft
	@echo "$(GREEN)Libft compiled$(RESET)"

clean: 
	rm -f $(OBJS)
	rm -f libft/*.o

fclean: clean 
	rm -f $(NAME)
	rm -f libft/libft.a

re:
	make fclean 
	make all

.PHONY: all clean fclean re







