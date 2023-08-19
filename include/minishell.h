#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum e_token_type
{
	T_WORD,             // Regular word/command/argument
	T_PIPE,             // |
	T_REDIRECT_IN,      // <
	T_REDIRECT_OUT,     // >
	T_REDIRECT_APPEND,  // >>
	T_REDIRECT_HEREDOC, // <<
	T_DOUBLE_QUOTE,     // " the whole sring in between " quotes included
	T_SINGLE_QUOTE,     // ' the whole string in between ' quotes included
	T_ENV_VAR,          // $ followed by a valid variable name
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
}					t_token;

t_token				*tokenizer(const char *input);
size_t				count_words_tokenizer(const char *input);