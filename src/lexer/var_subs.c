#include "minishell.h"

char	*lookup_env_value(char *var_name, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1); // Return the value part after the '='
		i++;
	}
	return (""); // bash returns an empty string if the variable is not found
}

t_lexeme	t_env_var_subs(t_token *token, t_data *data)
{
	t_lexeme lexeme;
	char *value;

	lexeme.original = ft_strdup(token->str);
	if (!lexeme.original)
		free_exit(data, "Error: malloc lexeme.original failed\n");
	value = lookup_env_value(token->str + 1, data->env_arr); //
	// +1 to skip the '$' if (value)
	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(value);
	if (!lexeme.str)
		free_exit(data, "Error: malloc lexeme.str failed\n");
	lexeme.status = NOT_LEXED;
	return (lexeme);
}