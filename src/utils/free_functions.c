#include "minishell.h"


void free_token_arr(t_token *token_arr)
{
    if (token_arr == NULL) 
		return;

    size_t i;
	i = 0;

    while (token_arr[i].type != T_END) 
	{
        if (token_arr[i].str != NULL) 
		{
            free(token_arr[i].str);
            token_arr[i].str = NULL;  // Optional, explained below
        }
        i++;
    }
    // free(token_arr[i].str);  // In case T_END token has a non-NULL str
    free(token_arr);
}

void	free_lexeme_arr(t_lexeme *lexeme_arr)
{
	size_t	i;

	i = 0;
	while (lexeme_arr[i].type != L_END)
	{
		if (lexeme_arr[i].str != NULL)
		{
			free(lexeme_arr[i].str);
			lexeme_arr[i].str = NULL;
			free(lexeme_arr[i].original);
			lexeme_arr[i].original = NULL;
		}
		i++;
	}
	free(lexeme_arr);
}