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

t_env_var	*create_node_ht(const char *key, const char *value)
{
	t_env_var	*node;

	node = (t_env_var *)malloc(sizeof(t_env_var));
	if (node == NULL)
	{
		// free memory
		exit(EXIT_FAILURE);
	}
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

void	insert_node_ht(t_env_var **table, const char *key, const char *value)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = table[idx];
	if (node == NULL)
	{
		table[idx] = create_node_ht(key, value);
	}
	else
	{
		if (node->key == NULL)
			printf("node->key == NULL\n");
		if (node->value == NULL)
			printf("node->value == NULL\n");
		while (node->next != NULL)
			node = node->next;
		node->next = create_node_ht(key, value);
	}
}

void	initialize_table(t_env_table *env_table, char **envp)
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
	// printf("Initializing table...\n");
	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		if (!key_value)
		{
			// free memory
			printf("Error: ft_split in init table failed\n");
			exit(EXIT_FAILURE);
		}
		if (!key_value[0])
		{
			// free memory
			printf("Error: key_value[0] == NULL\n");
			exit(EXIT_FAILURE);
		}
		if (!key_value[1])
			key_value[1] = ft_strdup("");
		insert_node_ht(env_table->table, key_value[0], key_value[1]);
		env_table->count++;
		free(key_value[0]);
		free(key_value[1]);
		free(key_value);
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

void	unset(t_env_table *env_table, const char *key)
{
	unsigned int	idx;
	t_env_var		*node;
	t_env_var		*prev;

	idx = hash(key);
	node = env_table->table[idx];
	prev = NULL;
	while (node != NULL)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
		{
			if (prev == NULL)
			{
				env_table->table[idx] = node->next;
			}
			else
			{
				prev->next = node->next;
			}
			free(node->key);
			free(node->value);
			free(node);
			env_table->count--;
			return ;
		}
		prev = node;
		node = node->next;
	}
}

char	**convert_hash_table_to_array(t_env_table *env_table)
{
	int i;
	int j;
	char **envp;
	char *temp;
	t_env_var *node;

	envp = (char **)malloc(sizeof(char *) * (env_table->count + 1));
	if (envp == NULL)
	{
		perror("Failed to allocate memory for envp");
		// free memory
		exit(EXIT_FAILURE);
	}
	i = 0;
	j = 0;
	while (i < TABLE_SIZE)
	{
		node = env_table->table[i];
		while (node != NULL)
		{
			temp = ft_strjoin(node->key, "=");
			envp[j] = ft_strjoin(temp, node->value);
			free(temp);
			j++;
			node = node->next;
		}
		i++;
	}
	envp[j] = NULL;
	return (envp);
}