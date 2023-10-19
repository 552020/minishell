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

t_lexeme	t_env_var_substitution(t_token *token, char **envp)
{
	t_lexeme lexeme;
	char *value;

	lexeme.original = ft_strdup(token->str);
	value = lookup_env_value(token->str + 1, envp); //
	// +1 to skip the '$' if (value)
	lexeme.type = L_ARGUMENT;
	lexeme.str = ft_strdup(value);
	lexeme.status = NOT_LEXED;
	return (lexeme);
}