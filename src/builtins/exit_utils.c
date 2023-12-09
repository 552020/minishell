/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:32:45 by slombard          #+#    #+#             */
/*   Updated: 2023/12/05 17:32:48 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parenthesis(t_ast_node *cmd)
{
	if (cmd->cmd[4] == '(' && cmd->cmd[ft_strlen(cmd->cmd) - 1] == ')')
		return (1);
	return (0);
}
