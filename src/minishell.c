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

void	free_after_execute(t_data *data)
{
	if (data->ast_root)
	{
		free_ast(data->ast_root);
		if (data->pipes_count > 0)
		{
			free_pipe_fds(data->pipe_fds, data->pipes_count);
			data->pipes_count = 0;
		}
		data->ast_root = NULL;
		data->ast_type = UNDEFINED;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;
	int		exit_status;

	check_input(argc, argv);
	initialize_data(envp, &data);
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
