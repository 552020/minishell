/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:23:34 by slombard          #+#    #+#             */
/*   Updated: 2023/12/08 23:48:17 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

char	*strip_ending_trailing_spaces(char const *str)
{
	char	*trimmed;
	int		end;

	if (!str)
		return (ft_strdup(""));
	end = ft_strlen(str) - 1;
	while (end >= 0 && ft_isspace(str[end]))
		end--;
	trimmed = ft_substr(str, 0, end + 1);
	return (trimmed);
}

t_token	*create_token_array(t_data *data)
{
	t_token	*token_arr;

	token_arr = (t_token *)ft_calloc(data->token_count + 1, sizeof(t_token));
	if (!token_arr)
		free_exit(data, "Error: ft_calloc failed\n");
	return (token_arr);
}

t_token	*tokenizer(t_data *data, const char *str)
{
	size_t	idx;

	idx = 0;
	while (*str)
	{
		skip_spaces(&str);
		if (isregularchar(*str, str))
			assign_word(&str, data, &idx);
		else if (*str == '<' || *str == '>')
			assign_redirect_in_out_heredoc_append(&str, data, &idx);
		else if (*str == '|')
			assign_pipe(&str, data, &idx);
		else if (*str == '$' && *(str + 1) == '?')
			assign_shell_var(&str, data, &idx);
		else if (*str == '$')
			assign_env_var(&str, data, &idx);
		else if (*str == '\'' || *str == '"')
			assign_quotes(&str, data, &idx);
		else
			handle_unexpected_char(&str);
	}
	data->token_arr[idx].type = T_END;
	data->token_arr[idx].str = NULL;
	return (data->token_arr);
}

void	tokenize(t_data *data, char *input)
{
	char	*trimmed;
	char	*tmp;
	char	*reshuffled;
	char	*expanded;

	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("\n***Tokenization***\n\n");
	trimmed = strip_ending_trailing_spaces(input);
	tmp = reshuffle_single_quotes(trimmed);
	reshuffled = reshuffle_double_quotes(tmp);
	// printf("Reshuffled: %s\n", reshuffled);
	free(tmp);
	free(input);
	expanded = wildcard_expansion(reshuffled, data);
	// printf("Expanded: %s\n", expanded);
	input = NULL;
	data->token_count = count_words_tokenizer(expanded);
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		printf("Token count: %zu\n\n", data->token_count);
	data->token_arr = create_token_array(data);
	data->token_arr = tokenizer(data, expanded);
	// if (reshuffled)
	// {
	// 	free(reshuffled);
	// 	reshuffled = NULL;
	// }
	if (DEBUG_LEVEL == DEBUG_ALL || DEBUG_LEVEL == DEBUG_TOKENIZER)
		print_token_arr(data->token_arr, data->token_count);
}
