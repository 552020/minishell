/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entries.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 04:56:13 by slombard          #+#    #+#             */
/*   Updated: 2023/12/10 05:21:38 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "wildcard.h"

typedef struct init_entries
{
	DIR				*dir;
	struct dirent	*dir_entry;
	int				idx;
	t_entry			**entries;
	int				count;
}					t_init_entries;

int	malloc_entries(t_init_entries *vars)
{
	vars->count = 0;
	vars->entries = NULL;
	vars->dir = opendir(".");
	if (!vars->dir)
		return (1);
	vars->dir_entry = readdir(vars->dir);
	while (vars->dir_entry != NULL)
	{
		if (ft_strncmp(vars->dir_entry->d_name, ".", 1) != 0
			|| ft_strncmp(vars->dir_entry->d_name, "..", 2) != 0)
			(vars->count)++;
		vars->dir_entry = readdir(vars->dir);
	}
	closedir(vars->dir);
	vars->entries = malloc(sizeof(t_entry *) * (vars->count + 1));
	if (!vars->entries)
		return (1);
	vars->entries[vars->count] = NULL;
	return (0);
}

void	assign_entry(t_init_entries *vars)
{
	if (ft_strncmp(vars->dir_entry->d_name, ".", 1) != 0
		|| ft_strncmp(vars->dir_entry->d_name, "..", 2) != 0)
	{
		vars->entries[vars->idx] = malloc(sizeof(t_entry));
		if (!vars->entries[vars->idx])
			return ;
		vars->entries[vars->idx]->entry = ft_strdup(vars->dir_entry->d_name);
		vars->entries[vars->idx]->idx = vars->entries[vars->idx]->entry;
		vars->idx++;
	}
}

void	init_entries(t_entries *entries_ptr)
{
	t_init_entries	vars;

	if (malloc_entries(&vars) != 0)
		return ;
	vars.dir = opendir(".");
	if (!vars.dir)
	{
		free(vars.entries);
		return ;
	}
	vars.idx = 0;
	vars.dir_entry = readdir(vars.dir);
	while (vars.dir_entry != NULL)
	{
		assign_entry(&vars);
		vars.dir_entry = readdir(vars.dir);
	}
	vars.entries[vars.idx] = NULL;
	closedir(vars.dir);
	entries_ptr->entries = vars.entries;
	entries_ptr->count = vars.count;
}
