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

t_env_var	*create_node_ht(const char *key, const char *value, t_free_data *free_data)
{
	t_env_var	*node;

	node = (t_env_var *)malloc(sizeof(t_env_var));
	if (node == NULL)
		free_exit(free_data, "Error: malloc in create_node_ht failed\n");
	node->key = ft_strdup(key);
	if (node->key == NULL)
		free_exit(free_data, "Error: ft_strdup in create_node_ht failed\n");
	node->value = ft_strdup(value);
	if (node->value == NULL)
		free_exit(free_data, "Error: ft_strdup in create_node_ht failed\n");
	node->next = NULL;
	return (node);
}

void	insert_node_ht(t_env_var **table, const char *key, const char *value, t_free_data *free_data)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = table[idx];
	if (node == NULL)
	{
		table[idx] = create_node_ht(key, value, free_data);
	}
	else
	{
		if (node->key == NULL)
			printf("node->key == NULL\n");
		if (node->value == NULL)
			printf("node->value == NULL\n");
		while (node->next != NULL)
			node = node->next;
		node->next = create_node_ht(key, value, free_data);
	}
}

void	initialize_table(t_env_table *env_table, char **envp, t_free_data *free_data)
{
	int		i;
	char	**key_value;

	i = 0;
	while (i < TABLE_SIZE)
	{
		env_table->table[i] = NULL;
		i++;
	}
	env_table->count = 0;
	i = 0;
	free_data->env_table = env_table;
	while (envp[i] != NULL)
	{
		key_value = ft_split_envp(envp[i], '=');
		if (!key_value)
			free_exit(free_data, "Error: ft_split in init table failed\n");
		if (!key_value[0])
			free_exit(free_data, "Error: ft_split in init table failed\n");
		// if (!key_value[1])
		// {	
		// 	key_value[1] = ft_strdup("");
		// }
		free_data->env_table = env_table;
		insert_node_ht(env_table->table, key_value[0], key_value[1], free_data);
		env_table->count++;
		free_key_value(key_value);
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

char	**convert_hash_table_to_array(t_env_table *env_table, t_free_data *free_data)
{
	int			i;
	int			j;
	char		**envp;
	char		*temp;
	t_env_var	*node;

	envp = (char **)malloc(sizeof(char *) * (env_table->count + 1));
	if (envp == NULL)
		free_exit(free_data, "Error: malloc in convert_hash_table_to_array failed\n");
	free_data->my_envp = envp;
	i = 0;
	j = 0;
	while (i < TABLE_SIZE)
	{
		node = env_table->table[i];
		while (node != NULL)
		{
			temp = ft_strjoin(node->key, "=");
			if (!temp)
				free_exit(free_data, "Error: ft_strjoin failed\n");
			envp[j] = ft_strjoin(temp, node->value);
			if (!envp[j])
			{	
				free(temp);
				free_exit(free_data, "Error: ft_strjoin failed\n");
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
}