/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 07:10:36 by bsengeze          #+#    #+#             */
/*   Updated: 2023/12/16 00:15:19 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	init_parser_vars(t_parser *vars, int start, int end)
{
	vars->i = end;
	vars->end = end;
	vars->start = start;
	vars->node = NULL;
}

int	find_parenthesis_sibling(t_lexeme *lexemes, int start, int end)
{
	int	i;
	int	parentheses_balance;

	(void)end;
	i = end;
	printf("find_parenthesis_sibling\n");
	printf("start: %d\n", start);
	printf("end: %d\n", end);
	parentheses_balance = 0;
	while (i >= start)
	{
		printf("i: %d\n", i);
		printf("lexemes[i].type: %d\n", lexemes[i].type);
		if (lexemes[i].type == L_PARENTHESIS_CLOSED)
			parentheses_balance++;
		else if (lexemes[i].type == L_PARENTHESIS_OPEN)
			parentheses_balance--;
		if (parentheses_balance == 0)
			return (i);
		i--;
	}
	return (-1);
}
