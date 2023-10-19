CC = gcc

NAME =	minishell 
CFLAGS = -g -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
LIBS = -lreadline
INCLUDES = -I./include  -I./libft/include
SRC_DIR = src
OBJ_DIR = obj

SRCS = minishell.c tokenizer.c tokenizer_utils.c tokenizer_assign.c lexer.c utils.c parser.c debug.c env_vars.c check_input.c \
		pipes.c tokenizer_count_words.c tokenizer_assign_redirect.c builtins/pwd.c builtins/env.c builtins/export.c builtins/unset.c read_input.c
# OBJS = $(SRCS:.c=.o)
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
# OBJS := $(OBJS:$(SRC_DIR)/%=$(OBJ_DIR)/%)
OBJS := $(OBJS:/=_)

all: $(NAME) $(LIBFT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# @mkdir -p $(OBJ_DIR)
	@mkdir -p $(@D)  
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 


$(NAME): $(OBJS) $(LIBFT) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

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







