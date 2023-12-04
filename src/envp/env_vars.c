#include "minishell.h"

char	**ft_split_envp(const char *s, char c, t_data *data)
{
	char	**ret;
	size_t	len;
	size_t	i;

	i = 0;
	ret = (char **)malloc(sizeof(char *) * (ft_count_word(s, c) + 2));
	if (!ret)
		free_exit(data, "Error: malloc failed\n");
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c && ++len)
				s++;
			ret[i] = ft_substr(s - len, 0, len);
			if (!ret[i])
			{
				ft_free_ret(ret, i);
				free_exit(data, "Error: malloc failed\n");
			}
			i++;
		}
		else
		{
			if (*(s + 1) == '\0')
				ret[i++] = ft_strdup("");
			s++;
		}
	}
	ret[i] = 0;
	return (ret);
}

void	initialize_table(char **envp, t_data *data)
{
	int		i;
	char	**key_value_pair;

	i = -1;
	while (++i < TABLE_SIZE)
		data->env_table->table[i] = NULL;
	data->env_table->count = 0;
	i = 0;
	while (envp[i] != NULL)
	{
		key_value_pair = ft_split_envp(envp[i], '=', data);
		if (!key_value_pair)
			free_exit(data, "Error: ft_split in init table failed\n");
		if (!key_value_pair[0])
			free_exit(data, "Error: ft_split in init table failed\n");
		data->env_table = data->env_table;
		insert_node_ht(key_value_pair[0], key_value_pair[1], data);
		data->env_table->count++;
		free_key_value_pair(key_value_pair);
		i++;
	}
}

char	*ft_getenv(t_env_var **table, const char *key)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = table[idx];
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			return (node->value);
		}
		node = node->next;
	}
	return (NULL); // key not found
}

char	**hash_table_to_arr(t_data *data)
{
	int			i;
	int			j;
	char		**envp;
	char		*temp;
	t_env_var	*node;

	if (data->env_table->count == 0)
		return (NULL);
	if (data->env_arr != NULL)
		free_envp(data->env_arr);
	envp = (char **)malloc(sizeof(char *) * (data->env_table->count + 1));
	if (envp == NULL)
		free_exit(data, "Error: malloc in hash_table_to_arr failed\n");
	data->env_arr = envp;
	i = 0;
	j = 0;
	while (i < TABLE_SIZE)
	{
		node = data->env_table->table[i];
		while (node != NULL)
		{
			temp = ft_strjoin(node->key, "=");
			if (!temp)
				free_exit(data, "Error: ft_strjoin failed\n");
			envp[j] = ft_strjoin(temp, node->value);
			if (!envp[j])
			{
				free(temp);
				temp = NULL;
				free_exit(data, "Error: ft_strjoin failed\n");
			}
			free(temp);
			temp = NULL;
			j++;
			node = node->next;
		}
		i++;
	}
	envp[j] = NULL;
	return (envp);
}
