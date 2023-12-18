CC = gcc

NAME =	minishell 
CFLAGS = -g -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
LIBS = -lreadline
INCLUDES = -I./include  -I./libft/include -I./src/wildcard
SRC_DIR = src
OBJ_DIR = obj

UNAME_S := $(shell uname -s)

# # Set include paths conditionally
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -fsanitize=address -g -O1
    INCLUDES = -I./include -I./libft/include -I/opt/homebrew/opt/readline/include -I./libft/include -I/usr/local/opt/readline/include -I./src/wildcard

else
    INCLUDES = -I./include -I./libft/include -I./src/wildcard
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
 tokenizer/tokenizer.c tokenizer/utils.c tokenizer/assign.c tokenizer/count_words.c tokenizer/assign_redirect.c tokenizer/reshuffle_quotes.c tokenizer/reshuffle_quotes_utils.c \
 wildcard/wildcard.c wildcard/build_pattern.c wildcard/build_pattern_init.c wildcard/free.c wildcard/utils.c wildcard/init_entries.c wildcard/matching.c wildcard/matching_utils.c \
 lexer/lexer.c lexer/lexemes_redirect.c lexer/lexemes.c lexer/quotes.c lexer/var_subs.c lexer/wrappers.c lexer/lexer_helper.c lexer/check_syntax.c lexer/lexer_helper.c lexer/quotes_helper.c \
 parser/parser.c parser/create_node.c parser/fill_node_cmd_args.c parser/fill_node_redirects.c parser/parser_utils.c \
 utils/check_input.c utils/free_functions.c utils/init.c \
 builtins/pwd.c builtins/env.c builtins/export.c builtins/unset.c builtins/cd.c builtins/exit.c builtins/echo.c builtins/exit_utils.c utils/read_input.c  \
 executor/redirections.c executor/heredoc.c executor/execute_builtins.c executor/executor.c executor/utils.c executor/handle_pipe.c executor/handle_single_cmd.c executor/ft_realpath.c executor/path_finder.c executor/cmd_and_args_arr.c \
 envp/env_vars.c envp/free.c envp/hash_table.c envp/utils.c \
 utils/ft_isvalidvarname.c tokenizer/reshuffle_quotes.c \
 utils/signals.c executor/wait.c debug/debug.c  tokenizer/reshuffle_quotes_utils.c utils/free_exit.c

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







