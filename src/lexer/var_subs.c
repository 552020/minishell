#include "minishell.h"

/*
The first return returns the value after the '=',
the second return returns an empty string if no value is found.
*/
char	*lookup_env_value(char *var_name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return ("");
}

t_lexeme	t_env_var_subs(t_token *token, t_data *data)
{
	t_lexeme	lexeme;
	char		*value;

	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	value = lookup_env_value(token->str + 1, data->env_arr); //
	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(value);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.status = NOT_LEXED;
	return (lexeme);
}

t_lexeme	t_shell_var_subs(t_token *token, t_data *data)
{
	t_lexeme lexeme;
	char *value;

	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	value = ft_itoa(data->last_exit_status);
	if (!value)
		free_exit(data, "Error: malloc value failed\n");
	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(value);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.status = NOT_LEXED;
	free(value);
	return (lexeme);
}