/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 22:30:22 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 22:59:29 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_echo
{
	int	i;
	int	j;
	int	print_newline;
}		t_echo;

void	init_t_echo(t_echo *echo)
{
	echo->i = 0;
	echo->j = 1;
	echo->print_newline = 1;
}

int	execute_without_args(void)
{
	printf("\n");
	return (EXIT_SUCCESS);
}

void	execute_with_n_flag(t_echo *echo, t_ast_node *node)
{
	while (node->args[echo->i] && ft_strncmp(node->args[echo->i], "-n", 2) == 0)
	{
		while (node->args[echo->i][echo->j]
			&& node->args[echo->i][echo->j] == 'n')
			echo->j++;
		if (node->args[0][echo->j] != 'n'
			&& (node->args[echo->i][echo->j] == ' '
				|| node->args[echo->i][echo->j] == '\0'))
			echo->print_newline = 0;
		if (node->args[echo->i][echo->j] == ' '
			|| node->args[echo->i][echo->j] == '\0')
			echo->i++;
		else
		{
			break ;
			return ;
		}
		echo->j = 1;
	}
}

int	echo(t_ast_node *node)
{
	t_echo	echo;

	init_t_echo(&echo);
	if (node->args == NULL)
		return (execute_without_args());
	if (ft_strncmp(node->args[0], "-n", 2) == 0)
		execute_with_n_flag(&echo, node);
	while (node->args[echo.i])
	{
		printf("%s", node->args[echo.i]);
		if (node->args[echo.i + 1])
			printf(" ");
		echo.i++;
	}
	if (echo.print_newline == 1)
		printf("\n");
	return (EXIT_SUCCESS);
}
