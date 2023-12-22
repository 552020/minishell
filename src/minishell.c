/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:45:46 by slombard          #+#    #+#             */
/*   Updated: 2023/12/22 00:44:26 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_main(t_data *data, int *exit_status)
{
	if (handle_heredocs(data->ast_root, data) == SUCCESS)
	{
		execute(data, data->ast_root);
		wait_ast(data, data->ast_root);
		*exit_status = data->ast_root->exit_status;
		data->last_exit_status = *exit_status;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	int		exit_status;

	check_input(argc, argv);
	initialize_data(envp, &data);
	data.debug_level = DEBUG_ALL;
	while (1)
	{
		handle_signals_main();
		input = read_input(&data);
		tokenize(&data, input);
		if (lexemize(&data) == SUCCESS)
		{
			parse(&data);
			execute_main(&data, &exit_status);
		}
		free_after_execute(&data);
	}
	return (exit_status);
}
