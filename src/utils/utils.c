/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slombard <slombard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:46:41 by slombard          #+#    #+#             */
/*   Updated: 2023/12/04 21:04:00 by slombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

int		ft_free_ret(char **ret, size_t i);

size_t	ft_count_word(const char *s, char c)
{
	size_t	i;
	size_t	words;

	i = 0;
	words = 0;
	if (!s[0])
		return (0);
	while (s[i] && s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			words++;
			while (s[i] && s[i] == c)
				i++;
		}
		else
			i++;
	}
	if (s[i - 1] != c)
		words++;
	return (words);
}

char	**ft_split_envp(const char *s, char c, t_data *data, t_split_envp *envs)
{
	while (*s)
	{
		if (*s != c)
		{
			envs->len = 0;
			while (*s && *s != c && ++envs->len)
				s++;
			envs->ret[envs->i] = ft_substr(s - envs->len, 0, envs->len);
			if (!envs->ret[envs->i])
			{
				ft_free_ret(envs->ret, envs->i);
				free_exit(data, "Error: malloc failed\n");
			}
			envs->i++;
		}
		else
		{
			if (*(s + 1) == '\0')
				envs->ret[envs->i++] = ft_strdup("");
			s++;
		}
	}
	envs->ret[envs->i] = 0;
	return (envs->ret);
}

void	ft_free_ret_envp(char **ret, size_t i)
{
	while (i--)
	{
		free(ret[i]);
		ret[i] = NULL;
	}
	free(ret);
	ret = NULL;
}
