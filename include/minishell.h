#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum e_token_type
{
	T_WORD,             // Regular word/command/argument
	T_PIPE,             // |
	T_REDIRECT_IN,      // <
	T_REDIRECT_OUT,     // >
	T_REDIRECT_APPEND,  // >>
	T_REDIRECT_HEREDOC, // <<
	T_DOUBLE_QUOTE,     // "
	T_SINGLE_QUOTE,     // '
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*str;
}					t_token;
