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

void	t_env_var_substitution(t_token *token, char **envp)
{
	if (token->type == T_ENV_VAR)
	{
		char *value = lookup_env_value(token->str + 1, envp); //
		// +1 to skip the '$' if (value)
		{
			free(token->str);
			token->str = ft_strdup(value);
		}
	}
}

void	t_double_quote_var_substitution(t_token *token, char **envp)
{
	char	*str;
	char	*end;
	char	*before;
	char	*var_name;
	char	*before_and_value;
	char	*value;
	char	*after;

	str = token->str;
	while ((str = ft_strchr(str, '$')))
	{
		end = str + 1;
		while (ft_isvalidvarname(*end))
			end++;
		before = ft_substr(token->str, 0, str - token->str);
		var_name = ft_substr(str, 1, end - str - 1);
		value = lookup_env_value(var_name, envp);
		after = ft_strdup(end);
		free(token->str);
		before_and_value = ft_strjoin(before, value);
		free(before);
		token->str = ft_strjoin(before_and_value, after);
		free(before_and_value);
		free(var_name);
		free(after);
		str = token->str + (str - token->str) + ft_strlen(value);
	}
}
