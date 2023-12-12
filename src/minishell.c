/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 17:45:46 by slombard          #+#    #+#             */
/*   Updated: 2023/12/05 17:45:52 by slombard         ###   ########.fr       */
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

/*
to debug add this line in main, before the while loop:
data.debug_level = DEBUG_ALL;
*/
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
		if (data.ast_root)
			free_ast(data.ast_root);
		data.ast_type = UNDEFINED;
	}
	return (exit_status);
}
