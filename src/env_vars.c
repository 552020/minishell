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

t_env_var	*create_node(const char *key, const char *value)
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

void	insert_node(t_env_var **table, const char *key, const char *value)
{
	unsigned int	idx;
	t_env_var		*node;

	idx = hash(key);
	node = table[idx];
	if (node == NULL)
	{
		table[idx] = create_node(key, value);
	}
	else
	{
		while (node->next != NULL)
		{
			node = node->next;
		}
		node->next = create_node(key, value);
	}
}

void	initialize_table(t_env_var **table, char **envp)
{
	int		i;
	char	**key_value;

	i = 0;
	while (envp[i] != NULL)
	{
		key_value = ft_split(envp[i], '=');
		insert_node(table, key_value[0], key_value[1]);
		free(key_value[0]);
		free(key_value[1]);
		free(key_value);
		i++;
	}
}

void	env(t_env_var **table)
{
	int			i;
	t_env_var	*node;

	i = 0;
	while (i < TABLE_SIZE)
	{
		node = table[i];
		while (node != NULL)
		{
			printf("%s=%s\n", node->key, node->value);
			node = node->next;
		}
		i++;
	}
}

void	export(t_env_var **table, const char *key, const char *value)
{
	t_env_var	*node;

	node = table[hash(key)];
	while (node != NULL)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			free(node->value);
			node->value = ft_strdup(value);
			return ;
		}
		node = node->next;
	}
	insert_node(table, key, value);
}

void	unset(t_env_var **table, const char *key)
{
	unsigned int idx;
	t_env_var *node;
	t_env_var *prev;

	idx = hash(key);
	node = table[idx];
	prev = NULL;
	while (node != NULL)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (prev == NULL)
			{
				table[idx] = node->next;
			}
			else
			{
				prev->next = node->next;
			}
			free(node->key);
			free(node->value);
			free(node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}