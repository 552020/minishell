/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 06:46:59 by slombard          #+#    #+#             */
/*   Updated: 2023/11/30 06:47:05 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(t_data *data)
{
	char	*input;

	input = readline("$> ");
	if (!input)
	{
		printf("exit\n");
		exit(0);
	}
	add_history(input);
	if (data->debug_level == DEBUG_ALL || data->debug_level == DEBUG_TOKENIZER)
		printf("readline: %s\n", input);
	return (input);
}
