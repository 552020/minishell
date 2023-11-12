#include "minishell.h"

unsigned int	hash(const char *key)
{
	unsigned int	hash;

	hash = 0;
	while (*key)
	{
		hash += *key++;
	}
	return (hash % TABLE_SIZE);
}

t_env_var	*create_node_ht(const char *key, const char *value, t_data *data)
{
	t_env_var	*node;

	node = (t_env_var *)malloc(sizeof(t_env_var));
	if (node == NULL)
		free_exit(data, "Error: malloc in create_node_ht failed\n");
	node->key = ft_strdup(key);
	if (node->key == NULL)
		free_exit(data, "Error: ft_strdup in create_node_ht failed\n");
	node->value = ft_strdup(value);
	if (node->value == NULL)
		free_exit(data, "Error: ft_strdup in create_node_ht failed\n");
	node->next = NULL;
	return (node);
}

void	insert_node_ht(const char *key, const char *value,
		t_data *data)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = data->env_table->table[idx];
	if (node == NULL)
	{
		data->env_table->table[idx] = create_node_ht(key, value, data);
	}
	else
	{
		if (node->key == NULL)
			printf("node->key == NULL\n");
		if (node->value == NULL)
			printf("node->value == NULL\n");
		while (node->next != NULL)
			node = node->next;
		node->next = create_node_ht(key, value, data);
	}
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
		key_value_pair = ft_split_envp(envp[i], '=');
		if (!key_value_pair)
			free_exit(data, "Error: ft_split in init table failed\n");
		if (!key_value_pair[0])
			free_exit(data, "Error: ft_split in init table failed\n");
		data->env_table = data->env_table;
		insert_node_ht(key_value_pair[0],
			key_value_pair[1], data);
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
				free_exit(data, "Error: ft_strjoin failed\n");
			}
			free(temp);
			j++;
			node = node->next;
		}
		i++;
	}
	envp[j] = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

void	free_hash_table(t_env_table *env_table)
{
	int i;
	t_env_var *node;
	t_env_var *temp;

	i = 0;
	while (i < TABLE_SIZE)
	{
		node = env_table->table[i];
		while (node != NULL)
		{
			temp = node;
			node = node->next;
			free(temp->key);
			free(temp->value);
			free(temp);
		}
		i++;
	}
	free(env_table);
}